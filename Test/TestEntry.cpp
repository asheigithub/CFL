// TestEntry.cpp : 
#define STRINGIFY(A)  #A

#include "CFLEntry.h"
#include "CFLFile.h"
#include "CFLString.h"
#include "CFLAsyncFunc.h"
#include "Content/CFLBinaryCache.h"
#include "Content/CFLMemoryStream.h"

#include "Content/CFLBuildInData.h"
#include "Content/CFLFileDataResource.h"
#include "Graphic/CFLVertexPointer.h"



#include "CFLDrawCommand.h"

#include <vector>
#include <random>


#include "Geom/CFLGeom.h"
#include "Geom/CFLRectangle.h"
#include "CFLMath.h"
#include "Content/CFLCTF.h"

#include "Content/CFLContent.h"
#include "Text/CFLEncoding.h"

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include "Graphic/CFLFont.h"

//adb shell top -m 10
//adb shell dumpsys meminfo com.app.TestEntry

void testfileread();
void testgeom();


using namespace cfl::tasks;
using namespace cfl;
cfl::CFLContext* context;


content::BuildInData getTestData()
{
	static GLfloat vVertices[] = { -0.5f, 0.5f, 0.0f,
		0.0f,0.0f,

		-0.5f, -0.5f, -0.0f,
		0.0f,1.0f,

		0.5f, -0.5f, -0.0f,
		1.0f,1.0f ,
	
		0.5f,0.5f, 0.0f,
		1.0f,0.0f

	};
	
	return content::BuildInData(vVertices, sizeof(vVertices));
}


float times = 0;
void up(CFLContext* context,float dettime)
{
	context->graphic->clear(0, 0, 0.0f, 1, 0, 0, cfl::graphic::ClearMaskBit::color);
	
	static int mmm = 0;

	times += dettime;
	//cfl::trace("%f\n", dettime);
	//if (times >= 1)
	//{
		//LOGI("%d\n", (int)cfl::getFPS());
		//times = 0;

		

		/*if (prog && prog->isValid())
		{
			LOGI("location: %d\n", prog->getAttribute("vPosition")->location);


		}*/

	//}
	
	


		auto iimg = content::Content::loadGameImage("testreadfile/commimage1/新资源/公用资源/竞技场-皇榜-上.png");
		auto iimg2 = content::Content::loadGameImage("testreadfile/commimage1/旗子导出2/白右/baiqiyou_00005.png");
		auto iimg3 = content::Content::loadGameImage("testreadfile/commimage1/界面文字/界面文字-标题/图鉴.png");
				

		context->graphic->drawGameImage(content::Content::loadGameImage("testreadfile/commimage1/主界面背景图.png"), 0.0f, 0.0f, 0.8f, nullptr, 1.0f);
		//context->graphic->drawGameImage(content::Content::loadGameImage("testreadfile/commimage1/主界面背景图.png"), 0.0f, 0.0f, 1.0f, nullptr, 1.0f);

		//context->graphic->testDrawVertex(prog, *vtx,*tex2d);
		for (size_t i = 0; i < 1; i++)
		{
			context->graphic->drawGameImage(iimg2, 0.0f, 0.0f ,1.0f, nullptr, 1.0f);

			context->graphic->drawGameImage(
				iimg, 140.0f, 400.0f , &geom::Matrix3D().appendRotation(toRadians(context->totalframes *1.0f / 60) * 15, geom::Y_AXIS), 1.0f, nullptr, 1.0f);// , context->totalframes *1.0f / 60 * PI );

				

			context->graphic->drawGameImage(iimg3, 0.0f, 0.0f, 1.0f, nullptr, 1.0f);
		}
			

		/*for (size_t i = 0; i < 100; i++)
		{
			context->graphic->drawLine(geom::Vector2(200, 200),
				geom::Vector2(200, 200) + geom::Vector2(300 * math::cosf(toRadians(i * 360.0f / 200)),
				200 *math::sinf(toRadians(i * 360.0f / 200))),

				graphic::Color(math::cosf((context->totalframes % 30)  * PI / 30 * 2)*0.5f + 0.5f,
				math::sinf((context->totalframes % 45)  * PI / 45 * 2)*0.5f + 0.5f,
				math::cosf((context->totalframes % 60)  * PI / 60 * 2)*0.5f + 0.5f, 1),

				((context->totalframes + i) % 200) / 10);
		}*/

			
		//context->graphic->testDrawVertex(prog, *vtx, *tex2d);

		for (size_t i = 0; i < 100; i++)
		{
			context->graphic->drawGameImage(iimg, 300.0f+i*10, 300.0f,
				((context->totalframes + i*3)  % 60)/60.0f ,nullptr,0.15f
				, toRadians( i*5 + context->totalframes )
				);

			context->graphic->drawGameImage(iimg2, 505.0f + i * 3, 167.0f, 1.0f, nullptr, i/10.0f * 0.5f, toRadians( i*context->totalframes/2) );


			context->graphic->drawGameImage(iimg3, 570.0f +i*20 , 426.0f + i * 10, 
				&geom::Matrix3D().appendRotation( toRadians(i*i+context->totalframes),geom::Y_AXIS ) ,1.0f, nullptr, i/100.0f+0.2f);
		}
		
		
		auto font = graphic::font::Font::getFont("arial");

		std::ostringstream oss;
		oss << "FPS:" << cfl::getFPS();

		context->graphic->drawString(oss.str().c_str(), font, 24, 0, 0);

		context->graphic->drawString(
			"泰文:" + CFLString(3626) + CFLString(3657) + CFLString(3657) + CFLString(3657) + CFLString(3657) + CFLString(3657),
			font, 32, 30, 50, nullptr
			);


		context->graphic->drawString(
			"组合音标[u+0063][u+0301]：" + CFLString(0x0063) + CFLString(0x0301),
			font,32,30,90
			);

		context->graphic->drawString(
			"火星文：(￣▽￣)~* ",
			font, 32, 30, 90+40
			);

		auto rcode = std::rand() % (4094) + 0x4e00;
		context->graphic->drawString(
			"每帧随机生成一个汉字：" + CFLString(rcode),
			font, 32, 30, 90 + 40*2
			);
		
		context->graphic->drawString(
			"实时高质量缩放：" ,
			font, 32, 30, 90 + 40 * 3
			);

		float scale = cfl::math::sinf (((context->totalframes) % 60) / 60.0f * PI * 2 ) * 5 + 0.5;
		context->graphic->drawString(
			"嘿",
			font, 32, 30+32*8, 90 + 40 * 3,
			nullptr,
			&cfl::geom::Matrix3D().appendScale(scale,scale,scale )
			);

		auto fcolor = cfl::graphic::Color(math::cosf((context->totalframes % 30)  * PI / 30 * 2)*0.5f + 0.5f,
			math::sinf((context->totalframes % 45)  * PI / 45 * 2)*0.5f + 0.5f,
			math::cosf((context->totalframes % 60)  * PI / 60 * 2)*0.5f + 0.5f, 1.0f);
		context->graphic->drawString(
			"字形三维旋转：",
			font, 64, 30, 150 + 40 * 4,
			&fcolor,
			&cfl::geom::Matrix3D().appendRotation(toRadians(context->totalframes ), geom::Y_AXIS)
			);

	
	testgeom();
	//testfileread();
	
	

};

//#define ACCESS _access  
//#define MKDIR(a) _mkdir((a))



void* testdo();
using namespace cfl;
int cfl_main(cfl::CFLContext* ctx, int argc, char* argv[])
{
	context = ctx;

	ctx->stage->scaleMode(StageScaleMode::SHOW_ALL);
	ctx->stage->stageWidth(1280);
	ctx->stage->stageHeight(720);

	cfl::registerUpdateFunc(ctx, up);

	//testdo();

	cfl::content::BinaryCache::getInstance()->setMaxCacheSize(1024 * 1024 * 2);

	{
		//文件流测试
		auto fs = cfl::file::getDirectory(file::DirType::asset)->getfile("testreadfile/file.txt")
			->openFileStreamForRead();

		//内存流测试
		/*auto fi = cfl::file::getDirectory(file::DirType::asset)->getfile("testreadfile/file.txt");
		cfl::file::FileData fd;
		fi->readFile(&fd);
		std::shared_ptr<content::memoryStream> fs = std::make_shared<content::memoryStream>(fd.data,2, fd.filesize);
		*/
		auto len = fs->getLength();
		LOGI("fs getLength");
		unsigned char* str = new unsigned char[len + 1];
		memset(str, 0, len + 1);

		str[0] = 'a';

		fs->seek(0, cfl::content::seekOrigin::begin);

		fs->read(str, 1, 2);
		LOGI("fs read: %s \n", str);

		fs->read(str, 0, 3);

		fs->setPosition(6);
		fs->read(str, 2, 100);


		fs->setPosition(0);
		auto byte = fs->readByte();
		int count = 0;
		while (byte > 0)
		{
			str[count] = byte;

			count++;
			byte = fs->readByte();
		}

		LOGI("fs len: %d, fs info: %s \n", len, str);

	}

	content::Content::initGameImageFromFile(file::DirType::asset, "testreadfile/commimage1/imgfilecfg.bin", "testreadfile/commimage1/");

	CFLString img = "testreadfile/commimage1/装饰纹案3/装饰纹案-3_05.png";

	/*auto gbkenc= cfl::text::Endoder::getGBK();
	auto string= gbkenc->getString("testreadfile/commimage1/装饰纹案3/装饰纹案-3_05.png");
	*/
	auto exists = content::Content::isExists(img);


	LOGI(" GameImage exists:%d, path: %s \n", exists, img.log_str());
	auto iimg = content::Content::loadGameImage("testreadfile/commimage1/icon_预览.png");
	iimg->refTexture->upload(true);

	UChar c('A');
	auto cat = UChar::getUnicodeCategory(c);

	auto cc = UChar::toLower(c);

	if (c == UChar::toUpper(cc))
	{
		LOGI(" %c==%c\n", c.charCode, cc.charCode);
	}

	LOGI(" %s,len: %d\n", img.log_str(), img.length());

	CFLString unichar = CFLString(c);

	auto len = unichar.length();
	auto acc = unichar.log_str();

	auto ucc2 = unichar + unichar;

	unsigned int  ucs4code[4] = { 0x0627, 0x0655, 0x0650, 0 };
	CFLString uni2(ucs4code);

	auto tick = cfl::getTimer();

	CFLString add("abcd哈哈");
	for (size_t i = 0; i < 100; i++)
	{
		uni2 += add;
	}
	len = uni2.length();

	auto t = cfl::getTimer() - tick;

	LOGI("timer: %d,%s\n", t, (uni2 + c).log_str());



	unsigned int  u4code[7] = { 0x0627, 0x0655, 0x0650, 0x064A, 0x0647, 0x064E, 0 };

	unsigned int u4code2[4] = { 0x0627, 0x0628, 0x064C, 0 };

	CFLString myString(u4code);

	text::TextElementEnumerator tee(myString + CFLString(u4code2));
	tee.reset();
	while (tee.moveNext())
	{
		auto te = tee.getCurrent();
		LOGI("st:%d,len:%d ", te->index, te->len);
		for (size_t i = te->index; i < te->index + te->len; i++)
		{
			LOGI("hex: %04X ", te->uchars[i]);
		}
		LOGI("\n");
	}




#ifdef ANDROID
	{
		auto fontfile = cfl::file::getDirectory(cfl::file::DirType::appStorage)->getfile("fonts/ARIAL.ff");
		if (!fontfile->isExists())
		{
			auto src = cfl::file::getDirectory(cfl::file::DirType::asset)->getfile("fonts/ARIAL.ff");
			cfl::file::FileData fd;
			src->readFile(&fd);
			fontfile->writeFile(fd.data, fd.filesize);
			fd.close();
		}
	}
	tick = cfl::getTimer();
	auto afont = cfl::graphic::font::Font::init(cfl::file::DirType::appStorage, "fonts/ARIAL.ff");
	t = cfl::getTimer() - tick;
	LOGI("font init timer: %d\n", t);

#else
	tick = cfl::getTimer();
	auto afont = cfl::graphic::font::Font::init(cfl::file::DirType::asset, "fonts/ARIAL.ff");
	t = cfl::getTimer() - tick;
	LOGI("font init timer: %d\n", t);
#endif

	tick = cfl::getTimer();
	for (size_t i = 0x21; i < 1024; i++)
	{
		auto k = afont.queryKerning(32, 932);

		//afont.getGlyphForRending(i);
	}

	t = cfl::getTimer() - tick;
	LOGI("query timer: %d\n", t);

	afont.getGlyphForRending('R');
	afont.getGlyphForRending('S');
	cfl::graphic::font::Font::regFont(afont, "arial");

#ifdef ANDROID
	{
		auto fontfile = cfl::file::getDirectory(cfl::file::DirType::appStorage)->getfile("fonts/FONT_DCT.ff");
		if (!fontfile->isExists())
		{
			auto src = cfl::file::getDirectory(cfl::file::DirType::asset)->getfile("fonts/FONT_DCT.ff");
			cfl::file::FileData fd;
			src->readFile(&fd);
			fontfile->writeFile(fd.data, fd.filesize);
			fd.close();
		}
	}

	tick = cfl::getTimer();
	auto hfont = cfl::graphic::font::Font::init(cfl::file::DirType::appStorage, "fonts/FONT_DCT.ff"); 
	cfl::graphic::font::Font::regFont(hfont, "simsun");
	t = cfl::getTimer() - tick;
	LOGI("simsun init timer: %d\n", t);
#else

	tick = cfl::getTimer();
	auto hfont = cfl::graphic::font::Font::init(cfl::file::DirType::asset, "fonts/FONT_DCT.ff");
	cfl::graphic::font::Font::regFont(hfont, "simsun");
	t = cfl::getTimer() - tick;
	LOGI("simsun init timer: %d\n", t);
#endif




#ifdef ANDROID
	{
		auto fontfile = cfl::file::getDirectory(cfl::file::DirType::appStorage)->getfile("fonts/TAI.ff");
		if (!fontfile->isExists())
		{
			auto src = cfl::file::getDirectory(cfl::file::DirType::asset)->getfile("fonts/TAI.ff");
			cfl::file::FileData fd;
			src->readFile(&fd);
			fontfile->writeFile(fd.data, fd.filesize);
			fd.close();
		}
	}
	{
		tick = cfl::getTimer();
		auto hfont = cfl::graphic::font::Font::init(cfl::file::DirType::appStorage, "fonts/TAI.ff");
		cfl::graphic::font::Font::regFont(hfont, "tai");
		t = cfl::getTimer() - tick;
		LOGI("simsun init timer: %d\n", t);
	}
#else
	{
		tick = cfl::getTimer();
		auto hfont = cfl::graphic::font::Font::init(cfl::file::DirType::asset, "fonts/TAI.ff");
		cfl::graphic::font::Font::regFont(hfont, "tai");
		t = cfl::getTimer() - tick;
		LOGI("simsun init timer: %d\n", t);
	}
#endif


	//render::textures::Texture2D texture;
	//texture.initFromFile(file::DirType::asset, "testreadfile/ktx888.ktx",false);

	
	//auto t = node.data();


	/*auto ctfheadersize = sizeof(content::CTFHeader);

	auto rgbadata=cfl::content::BinaryCache::getInstance()->getData(cfl::file::DirType::asset, "testreadfile/outrgba.dat");
	auto data = rgbadata.getData();
	
	content::CTFHeader header;
	memcpy(&header, data, ctfheadersize);

	data += ctfheadersize;
	
	CFL_ASSERT(header.length == rgbadata.getResourceSize() - ctfheadersize);*/

	


	//unsigned char str[] = {0xe4, 0xb8, 0xad, 0xe6, 0x96, 0x87, 0xe8, 0xb7, 0xaf, 0xe5, 0xbe, 0x84, 0x2f, 0x54, 0x68, 0x75,
	//	0x6e, 0x64, 0x65, 0x72, 0x2e, 0x74, 0x78, 0x74,0}; //"中文路径/Thunder.txt"; 成功获取中文路径文件。说明路径需转为utf8
	//
	//auto file = cfl::file::getDirectory(cfl::file::DirType::document)->getfile( reinterpret_cast< char*>( str));

	//cfl::trace("file: %s,isExists %d \n",file->getFilePath() ,file->isExists());

	/*char* k = "olé";
	std::hash< char*> hash;
	auto s = hash(k);*/
	
	
	//CFL_ASSERT(p1 == p2);
	//CFL_ASSERT(p1 == p3);
	
	
	return 0;
}

void testgeom()
{
	geom::Matrix mat(3.0f, 4.56f, -5, 6, 9, -4);

	//auto mat2 = mat;

	//mat.identity();
	mat.rotate(toRadians(15));

	mat.scale(5, 13);

	mat.translate(30, -40);

	mat.rotate(toRadians(-73));

	//mat2.append(mat);

	auto inv = mat.getInvert();

	auto test = mat * inv;

	auto v = mat.transformPoint(geom::Vector2(40.44f, 55.061f));
	//(x=5239.566531818498, y=2993.9335602476685)

	auto v2 = inv.transformPoint(v);

	auto k = geom::Vector2(40.44f, 55.061f) * mat;
	//LOGI("x=%f,y=%f\n", k.x, k.y);

	//CFL_ASSERT(geom::Vector2(40.44f, 55.061f) == v2);

	geom::Matrix3D mat3d(mat);
	
	auto det = mat3d.determinant();//应该等于determinant = 2651.999755859375
	
	auto mat3dinv = mat3d.getInvert();
	
	auto testidentity = mat3d * mat3dinv;


	//mat3d.identity();
	//mat.identity();

	//mat.rotate(toRadians(25));

	mat3d.appendRotation(toRadians(25), geom::Y_AXIS);
	mat3d.appendScale(3, 2, 1);
	mat3d.appendRotation(toRadians(-12), geom::X_AXIS);

	auto tovec = geom::Vector4(3, 2, 1) * mat3d;
	//Vector3D(707.3577880859375, 49.67710876464844, -121.83626556396484)
	mat3d.invert();
	auto invp = tovec * mat3d;


	geom::Quaternion qu1; qu1.fromAxisAngle(geom::Vector3(1.5, 0.5, 33), toRadians(55));

	geom::Matrix3D toro;
	
	qu1.setMatrix3D(&toro);

	geom::Quaternion qu2; qu2.fromMatrix3D(toro);

	

	geom::Rectangle firstRect= geom::Rectangle();
	//LOGI("%s\n",firstRect.toString().log_str());  // (x=0, y=0, w=0, h=0)
	geom::Rectangle secondRect = geom::Rectangle(1, 3, 11, 13);
	//LOGI("%s\n", secondRect.toString().log_str()); // (x=1, y=3, w=11, h=13)
	geom::Rectangle thirdRect = geom::Rectangle(5, 8, 17, 19);
	//LOGI("%s\n", thirdRect.toString().log_str());  // (x=5, y=8, w=17, h=19)
	bool isContained  = secondRect.containsRect(thirdRect);
	//LOGI(isContained); // false


}


void testfileread()
{


	auto bincache = cfl::content::BinaryCache::getInstance();

	std::vector<CFLString> toreadfiles(
	{
		"1", "2", "3", "4", "5", "6", "7", "8", "9", "big" }
	);
	std::vector<CFLString> files2({
		"diffuse0.gif.tgz", "lod_1.ass.tgz", "lod_2.ass.tgz", "model.ass.tgz"
	});

	std::vector<CFLString> bigs({
		"Battlefield01.mp3.tgz", "LIMPIDLUSTER.mp3.tgz", "Login.mp3.tgz"
		, "Map.mp3.tgz", "Scene.mp3.tgz"
	});

	std::srand(getTimer());


	for (size_t i = 0; i < 15; i++)
	{
		auto rnd = std::rand() % 3;

		auto p1 = toreadfiles[std::rand() % (toreadfiles.size())];
		auto p2 = files2[std::rand() % (files2.size())];

		if (p1 == CFLString("big"))
		{
			p2 = bigs[std::rand() % (bigs.size())];
		}

		auto fullpath = "testreadfile/" + p1 + "/" + p2;

		

		if (rnd == 2)
		{

			auto k = content::BinaryCache::getInstance()->getData(file::DirType::asset, "testreadfile/file.txt");
			CFL_ASSERT(CFLString(k.getData()) == CFLString("内容"));
			auto kd = k.getData();
			//trace("%s \n", kd);



		}
		else
			if (rnd == 0 && false)
			{
				auto gf = [](CFLString fullpath, cfl::content::BinaryCache* bincache)
				{
					try
					{
						auto data = bincache->getData(file::DirType::asset, fullpath);
						CFL_ASSERT(data.getStatus() == file::FileIoStatus::success);
						if (data.getStatus() != file::FileIoStatus::success)
						{
							auto cc = data.getResoucePath().c_str();

							LOGE("sync filepath:%s error!! \n", fullpath.log_str());
						}
						else
						{
							auto cc = data.getResoucePath();
							//LOGI("sync filepath:%s success \n", cc.c_str());
						}
					}
					catch (content::BinaryDataDeathLockException ex)
					{
						LOGE("%s\n", ex.what());
					}

				};
				//for (size_t i = 0; i < 20; i++)
				{

					asyncFunc::beginInvoke(gf, fullpath, bincache);


				}

				//trace("call begininvoke %s\n",fullpath.c_str());
			}
			else
			{
				//trace("async %s\n", fullpath.c_str());
				auto h = bincache->getDataAsync(file::DirType::asset, fullpath
					,
					[](content::BinaryData* data,void* args)
				{
					auto path = data->getResoucePath().c_str();
					auto prog = (float)(data->getReadSize()) / data->getResourceSize();

					//LOGI("filepath:%s,progress: %f\n", data->getResoucePath().c_str(), (float)(data->getReadSize()) / data->getResourceSize());
				}
					,
					[](content::BinaryData* data,void* args)
				{
					if (data->getStatus() != file::FileIoStatus::success)
					{
						CFL_ASSERT(false);

						//LOGE("filepath:%s error!! \n", data->getResoucePath().c_str());

					}
					else
					{
						auto path = data->getResoucePath().c_str();

						//LOGI("filepath:%s over \n", data->getResoucePath().c_str());
					}

					int* value= reinterpret_cast<int*>(args);
					//LOGI("value: %d\n", *value);

					delete value;
				},
					new int(i)
					);
				
			}
	}

	auto s = cfl::content::BinaryCache::getInstance()->getCachedSize();

}


