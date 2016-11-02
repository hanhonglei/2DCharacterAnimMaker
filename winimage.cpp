//
// WINIMAGE.CPP : Generic classes for raster images (MSWindows specialization)
//
//  Content: Member definitions for:
//  - class C_Image             : Storage class for single images
//  - class C_ImageSet          : Storage class for sets of images
//  - class C_AnimationWindow   : Window Class to display animations
//
//  (Includes routines to Load and Save BMP files and to load GIF files into
// these classes).
//
//  --------------------------------------------------------------------------
//
// Copyright ?2000, Juan Soulie <jsoulie@cplusplus.com>
//
// Permission to use, copy, modify, distribute and sell this software or any
// part thereof and/or its documentation for any purpose is granted without fee
// provided that the above copyright notice and this permission notice appear
// in all copies.
//
// This software is provided "as is" without express or implied warranty of
// any kind. The author shall have no liability with respect to the
// infringement of copyrights or patents that any modification to the content
// of this file or this file itself may incur.
//
// ****************************************************************************

#include <windows.h>
//#include <fstream.h>
#include <fstream>
#include "winimage.h"
using namespace std;

// Error processing macro (NO-OP by default):
#define ERRORMSG(PARAM) {}

// ****************************************************************************
// * C_Image Member definitions                                               *
// ****************************************************************************

// Init: Allocates space for raster and palette in GDI-compatible structures.
void C_Image::Init(int iWidth, int iHeight, int iBPP)
{
	if (Raster) {delete[]Raster;Raster=0;}
	if (pbmi) {delete[]pbmi;pbmi=0;}
	// Standard members setup
	Transparent=-1;
	BytesPerRow = Width = iWidth; Height=iHeight; BPP=iBPP;
	// Animation Extra members setup:
	xPos=xPos=Delay=0;

	if (BPP==24)
		{BytesPerRow*=3; pbmi=(BITMAPINFO*)new char [sizeof(BITMAPINFO)];}
	else
	{
		pbmi=(BITMAPINFO*)
			new char[sizeof(BITMAPINFOHEADER)+(1<<BPP)*sizeof(COLOR)];
		Palette=(COLOR*)((char*)pbmi+sizeof(BITMAPINFOHEADER));
	}

	BytesPerRow += (ALIGN-Width%ALIGN) % ALIGN;	// Align BytesPerRow
	
	Raster = new unsigned char [BytesPerRow*Height];

	pbmi->bmiHeader.biSize=sizeof (BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth=Width;
	pbmi->bmiHeader.biHeight=-Height;			// negative means up-to-bottom 
	pbmi->bmiHeader.biPlanes=1;
	pbmi->bmiHeader.biBitCount=(BPP<8?8:BPP);	// Our raster is byte-aligned
	pbmi->bmiHeader.biCompression=BI_RGB;
	pbmi->bmiHeader.biSizeImage=0;
	pbmi->bmiHeader.biXPelsPerMeter=11811;
	pbmi->bmiHeader.biYPelsPerMeter=11811;
	pbmi->bmiHeader.biClrUsed=0;
	pbmi->bmiHeader.biClrImportant=0;
}

// ****************************************************************************
// * C_ImageSet Member definitions                                            *
// ****************************************************************************

// AddImage: Adds an image object to the back of the img vector.
void C_ImageSet::AddImage (C_Image* newimage)
{
	C_Image ** pTempImg = new C_Image* [nImages+1];
	int n = 0;
	for (n=0;n<nImages;n++) pTempImg[n]=img[n];	// (pointer copy)
	delete[] img;
	img=pTempImg;
	img[n]=newimage;
	nImages++;
}

// pre-declaration:
int LZWDecoder (char*, char*, short, int, int, int, const int);

// ****************************************************************************
// * LoadGIF                                                                  *
// *   载入一个GIF文件到C_ImageSet对象里面                             *
// *                        (c) Nov 2000, Juan Soulie <jsoulie@cplusplus.com> *
// ****************************************************************************
int C_ImageSet::LoadGIF (char * szFileName)
{
	int		n;

	// GIF全程变量:
	int		GlobalBPP;						// 每个像素的位数.
	COLOR	* GlobalColorMap;				// 全局颜色列表 (allocate)

	struct GIFGCEtag {				// 图像控制扩展块——GRAPHIC CONTROL EXTENSION
		unsigned char BlockSize;		// 块大小: 4 bytes
		unsigned char PackedFields;		// 填充块区域每位的细节Packed Fields. Bits detail:
										//    0: 透明颜色标志Transparent Color Flag
										//    1: 用户输入标志User Input Flag
										//  2-4: 处理办法Disposal Method
		unsigned short Delay;			// 延迟时间Delay Time (1/100 seconds)
		unsigned char Transparent;		// 透明颜色索引Transparent Color Index
	} gifgce;
	int GraphicExtensionFound = 0;

	// 打开文件
	ifstream giffile (szFileName,ios::in/*|ios::nocreate*/|ios::binary);
	if (!giffile.is_open()) {ERRORMSG("File not found");return 0;}

	// 1、读数据头 *1* READ HEADER (SIGNATURE + VERSION)
	char szSignature[6];				// 前6个字 First 6 bytes (GIF87a or GIF89a)
	giffile.read(szSignature,6);
	if ( memcmp(szSignature,"GIF",2) != 0)
		{ ERRORMSG("Not a GIF File"); return 0; }

	// 2、读逻辑屏幕描述 *2* READ LOGICAL SCREEN DESCRIPTOR
	struct GIFLSDtag {
		unsigned short ScreenWidth;		// 屏幕宽度Logical Screen Width
		unsigned short ScreenHeight;	// 屏幕高度Logical Screen Height
		unsigned char PackedFields;		// 填充区每位的意义Packed Fields. Bits detail:
										//  0-2: 全局颜色表的大小Size of Global Color Table
										//    3: 分类标志(Sort Flag)，如果置位表示全局颜色列表分类排列Sort Flag
										//  4-6: 颜色深度Color Resolution
										//    7:全局颜色列表标志 Global Color Table Flag
		unsigned char Background;		// 背景颜色Background Color Index
		unsigned char PixelAspectRatio;	// 像素宽高比Pixel Aspect Ratio
	} giflsd;

	giffile.read((char*)&giflsd,sizeof(giflsd));

	GlobalBPP = (giflsd.PackedFields & 0x07) + 1;

	// 赋值给一些动画数据fill some animation data:
	FrameWidth = giflsd.ScreenWidth;
	FrameHeight = giflsd.ScreenHeight;
	nLoops = 0;

	// 3、读取/生成全局颜色列表*3* READ/GENERATE GLOBAL COLOR MAP
	GlobalColorMap = new COLOR [1<<GlobalBPP];
	if (giflsd.PackedFields & 0x80)	// File has global color map?
		for (n=0;n< 1<<GlobalBPP;n++)
		{
			GlobalColorMap[n].r=giffile.get();
			GlobalColorMap[n].g=giffile.get();
			GlobalColorMap[n].b=giffile.get();
		}

	else	// GIF标准提供内部默认的颜色表 GIF standard says to provide an internal default Palette:
		for (n=0;n<256;n++)
			GlobalColorMap[n].r=GlobalColorMap[n].g=GlobalColorMap[n].b=n;

	// 4、有3种可能*4* NOW WE HAVE 3 POSSIBILITIES:
	//  4a) 得到一个扩展块（附加信息块）Get and Extension Block (Blocks with additional information)
	//  4b) 得到一个图形解析（图形信息）Get an Image Separator (Introductor to an image)
	//  4c) GIF文件结束标志Get the trailer Char (End of GIF File)
	do
	{
		int charGot = giffile.get();

		if (charGot == 0x21)		// *A* 扩展块 EXTENSION BLOCK 
		{
			switch (giffile.get())
			{

			case 0xF9:			// Graphic Control Extension

				giffile.read((char*)&gifgce,sizeof(gifgce));
				GraphicExtensionFound++;
				giffile.get(); // Block Terminator (always 0)
				break;

			case 0xFE:			// Comment Extension: Ignored
			case 0x01:			// PlainText Extension: Ignored
			case 0xFF:			// Application Extension: Ignored
			default:			// Unknown Extension: Ignored
				// read (and ignore) data sub-blocks
				while (int nBlockLength = giffile.get())
					for (n=0;n<nBlockLength;n++) giffile.get();
				break;
			}
		}


		else if (charGot == 0x2c) {	// *B* 图形 IMAGE (0x2c Image Separator)

			// 生成一个新的C_Image对象 Create a new Image Object:
			C_Image* NextImage;
			NextImage = new C_Image;

			// 读取图像描述 Read Image Descriptor
			struct GIFIDtag {	
				unsigned short xPos;			// 图像左边位置 Image Left Position
				unsigned short yPos;			// 图像右边位置 Image Top Position
				unsigned short Width;			// 图像宽度 Image Width
				unsigned short Height;			// 图像高度 Image Height
				unsigned char PackedFields;		// 填充区域每位的意义 Packed Fields. Bits detail:
											//  0-2: 局部颜色列表大小(Size of Local Color Table)，pixel+1就为颜色列表的位数
											//  3-4: 保留(Reserved)
											//    5: 分类标志(Sort Flag)，如果置位表示紧跟着的局部颜色列表分类排列Sort Flag
											//    6: 交织标志(Interlace Flag)，置位时图象数据使用交织方式排列（详细描述...），否则使用顺序排列Interlace Flag
											//    7: 部颜色列表标志(Local Color Table Flag)Local Color Table Flag
			} gifid;

			giffile.read((char*)&gifid, sizeof(gifid));

			int LocalColorMap = (gifid.PackedFields & 0x08)? 1 : 0;

			NextImage->Init (gifid.Width, gifid.Height,
				LocalColorMap ? (gifid.PackedFields&7)+1 : GlobalBPP);

			// Fill NextImage Data
			NextImage->xPos = gifid.xPos;
			NextImage->yPos = gifid.yPos;
			if (GraphicExtensionFound)
			{
				NextImage->Transparent=
					(gifgce.PackedFields&0x01) ? gifgce.Transparent : -1;
				NextImage->Transparency=
					(gifgce.PackedFields&0x1c)>1 ? 1 : 0;
				NextImage->Delay = gifgce.Delay*10;
			}
		
			if (LocalColorMap)		// Read Color Map (if descriptor says so)
				giffile.read((char*)NextImage->Palette,
					sizeof(COLOR)*(1<<NextImage->BPP));

			else					// Otherwise copy Global
				memcpy (NextImage->Palette, GlobalColorMap,
					sizeof(COLOR)*(1<<NextImage->BPP));

			short firstbyte=giffile.get();	// 1st byte of img block (CodeSize)

			// Calculate compressed image block size
				// to fix: this allocates an extra byte per block
			long ImgStart,ImgEnd;				
			ImgEnd = ImgStart = giffile.tellg();
			while (n=giffile.get()) giffile.seekg (ImgEnd+=n+1);
			giffile.seekg (ImgStart);

			// Allocate Space for Compressed Image
			char * pCompressedImage = new char [ImgEnd-ImgStart+4];
  
			// Read and store Compressed Image
			char * pTemp = pCompressedImage;
			while (int nBlockLength = giffile.get())
			{
				giffile.read(pTemp,nBlockLength);
				pTemp+=nBlockLength;
			}

			// Call LZW/GIF decompressor
			n=LZWDecoder(
				(char*) pCompressedImage,
				(char*) NextImage->Raster,
				firstbyte, NextImage->BytesPerRow,//NextImage->AlignedWidth,
				gifid.Width, gifid.Height,
				((gifid.PackedFields & 0x40)?1:0)	//Interlaced?
				);

			if (n)
				AddImage(NextImage);
			else
			{
				delete NextImage;
				ERRORMSG("GIF File Corrupt");
			}

			// Some cleanup
			delete[]pCompressedImage;
			GraphicExtensionFound=0;
		}


		else if (charGot == 0x3b) {	// *C* TRAILER: End of GIF Info
			break; // Ok. Standard End.
		}

	} while (giffile.good());

	giffile.close();
	if (nImages==0) ERRORMSG("Premature End Of File");
	return nImages;
}

// ****************************************************************************
// * LZWDecoder (C/C++)                                                       *
// * Codec to perform LZW (GIF Variant) decompression.                        *
// *                         (c) Nov2000, Juan Soulie <jsoulie@cplusplus.com> *
// ****************************************************************************
//
// Parameter description:
//  - bufIn: Input buffer containing a "de-blocked" GIF/LZW compressed image.
//  - bufOut: Output buffer where result will be stored.
//  - InitCodeSize: Initial CodeSize to be Used
//    (GIF files include this as the first byte in a picture block)
//  - AlignedWidth : Width of a row in memory (including alignment if needed)
//  - Width, Height: Physical dimensions of image.
//  - Interlace: 1 for Interlaced GIFs.
//
int LZWDecoder (char * bufIn, char * bufOut,
				short InitCodeSize, int AlignedWidth,
				int Width, int Height, const int Interlace)
{
	int n;
	int row=0,col=0;				// used to point output if Interlaced
	int nPixels, maxPixels;			// Output pixel counter

	short CodeSize;					// Current CodeSize (size in bits of codes)
	short ClearCode;				// Clear code : resets decompressor
	short EndCode;					// End code : marks end of information

	long whichBit;					// Index of next bit in bufIn
	long LongCode;					// Temp. var. from which Code is retrieved
	short Code;						// Code extracted
	short PrevCode;					// Previous Code
	short OutCode;					// Code to output

	// Translation Table:
	short Prefix[4096];				// Prefix: index of another Code
	unsigned char Suffix[4096];		// Suffix: terminating character
	short FirstEntry;				// Index of first free entry in table
	short NextEntry;				// Index of next free entry in table

	unsigned char OutStack[4097];	// Output buffer
	int OutIndex;					// Characters in OutStack

	int RowOffset;					// Offset in output buffer for current row

	// Set up values that depend on InitCodeSize Parameter.
	CodeSize = InitCodeSize+1;
	ClearCode = (1 << InitCodeSize);
	EndCode = ClearCode + 1;
	NextEntry = FirstEntry = ClearCode + 2;

	whichBit=0;
	nPixels = 0;
	maxPixels = Width*Height;
	RowOffset =0;

	while (nPixels<maxPixels) {
		OutIndex = 0;							// Reset Output Stack

		// GET NEXT CODE FROM bufIn:
		// LZW compression uses code items longer than a single byte.
		// For GIF Files, code sizes are variable between 9 and 12 bits 
		// That's why we must read data (Code) this way:
		LongCode=*((long*)(bufIn+whichBit/8));	// Get some bytes from bufIn
		LongCode>>=(whichBit&7);				// Discard too low bits
		Code =(LongCode & ((1<<CodeSize)-1) );	// Discard too high bits
		whichBit += CodeSize;					// Increase Bit Offset

		// SWITCH, DIFFERENT POSIBILITIES FOR CODE:
		if (Code == EndCode)					// END CODE
			break;								// Exit LZW Decompression loop

		if (Code == ClearCode) {				// CLEAR CODE:
			CodeSize = InitCodeSize+1;			// Reset CodeSize
			NextEntry = FirstEntry;				// Reset Translation Table
			PrevCode=Code;				// Prevent next to be added to table.
			continue;							// restart, to get another code
		}
		if (Code < NextEntry)					// CODE IS IN TABLE
			OutCode = Code;						// Set code to output.

		else {									// CODE IS NOT IN TABLE:
			OutIndex++;			// Keep "first" character of previous output.
			OutCode = PrevCode;					// Set PrevCode to be output
		}

		// EXPAND OutCode IN OutStack
		// - Elements up to FirstEntry are Raw-Codes and are not expanded
		// - Table Prefices contain indexes to other codes
		// - Table Suffices contain the raw codes to be output
		while (OutCode >= FirstEntry) {
			if (OutIndex > 4096) return 0;
			OutStack[OutIndex++] = Suffix[OutCode];	// Add suffix to Output Stack
			OutCode = Prefix[OutCode];				// Loop with preffix
		}

		// NOW OutCode IS A RAW CODE, ADD IT TO OUTPUT STACK.
		if (OutIndex > 4096) return 0;
		OutStack[OutIndex++] = (unsigned char) OutCode;

		// ADD NEW ENTRY TO TABLE (PrevCode + OutCode)
		// (EXCEPT IF PREVIOUS CODE WAS A CLEARCODE)
		if (PrevCode!=ClearCode) {
			Prefix[NextEntry] = PrevCode;
			Suffix[NextEntry] = (unsigned char) OutCode;
			NextEntry++;

			// Prevent Translation table overflow:
			if (NextEntry>=4096) return 0;
      
			// INCREASE CodeSize IF NextEntry IS INVALID WITH CURRENT CodeSize
			if (NextEntry >= (1<<CodeSize)) {
				if (CodeSize < 12) CodeSize++;
				else {}				// Do nothing. Maybe next is Clear Code.
			}
		}

		PrevCode = Code;

		// Avoid the possibility of overflow on 'bufOut'.
		if (nPixels + OutIndex > maxPixels) OutIndex = maxPixels-nPixels;

		// OUTPUT OutStack (LAST-IN FIRST-OUT ORDER)
		for (n=OutIndex-1; n>=0; n--) {
			if (col==Width)						// Check if new row.
			{
				if (Interlace) {				// If interlaced::
					     if ((row&7)==0) {row+=8; if (row>=Height) row=4;}
					else if ((row&3)==0) {row+=8; if (row>=Height) row=2;}
					else if ((row&1)==0) {row+=4; if (row>=Height) row=1;}
					else row+=2;
				}
				else							// If not interlaced:
					row++;

				RowOffset=row*AlignedWidth;		// Set new row offset
				col=0;
			}
			bufOut[RowOffset+col]=OutStack[n];	// Write output
			col++;	nPixels++;					// Increase counters.
		}

	}	// while (main decompressor loop)

	return whichBit;
}

// Refer to WINIMAGE.TXT for copyright and patent notices on GIF and LZW.
