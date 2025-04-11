ROOT_DIR = .

.IGNORE:

.\files\default.chm : .\help\en-us\contents\* .\files\utils\SAVEreg.bat
	cd .\help\en-us
	hhc help_en.hhp
	cd ..\..
	copy .\help\en-us\en.chm .\files\default.chm

.\files\en.chm : .\help\en-us\contents\* .\files\utils\SAVEreg.bat
	cd .\help\en-us
	hhc help_en.hhp
	cd ..\..
	copy .\help\en-us\en.chm .\files

.\files\ja-JP.chm : .\help\ja-jp\contents\* .\files\utils\SAVEreg.bat
	cd .\help\ja-jp
	hhc .\help_ja-jp.hhp
	cd ..\..
	copy .\help\ja-JP\ja-JP.chm .\files

.\files\ko-KR.chm : .\help\ko-kr\contents\* .\files\utils\SAVEreg.bat
	cd .\help\ko-kr
	hhc help_ko-kr.hhp
	cd ..\..
	copy .\help\ko-kr\ko-KR.chm .\files

.\files\pt-BR.chm : .\help\pt-br\contents\* .\files\utils\SAVEreg.bat
	cd .\help\pt-br
	hhc help_br.hhp
	cd ..\..
	copy .\help\pt-br\pt-BR.chm .\files

.\files\zh-CN.chm : .\help\zh-cn\contents\* .\files\utils\SAVEreg.bat
	cd .\help\zh-cn
	hhc help_zh-cn.hhp
	cd ..\..
	copy .\help\zh-cn\zh-CN.chm .\files

.\files\zh-TW.chm : .\help\zh-tw\contents\* .\files\utils\SAVEreg.bat
	cd .\help\zh-tw
	hhc help_zh-tw.hhp
	cd ..\..
	copy .\help\zh-tw\zh-TW.chm .\files

!CMDSWITCHES

.\files\utils\SAVEreg.bat :
	mkdir .\files\utils
	copy .\util\SAVEreg.bat .\files\utils\SAVEreg.bat

.\files\default.lng : .\lang\default.lng .\files\utils\SAVEreg.bat
	copy .\lang\default.lng .\files

.\files\en.lng : .\lang\en.lng .\files\utils\SAVEreg.bat
	copy .\lang\en.lng .\files

.\files\ja-JP.lng : .\lang\ja-JP.lng .\files\utils\SAVEreg.bat
	copy .\lang\ja-JP.lng .\files

.\files\ko-KR.lng : .\lang\ko-KR.lng .\files\utils\SAVEreg.bat
	copy .\lang\ko-KR.lng .\files

.\files\pt-BR.lng : .\lang\pt-BR.lng .\files\utils\SAVEreg.bat
	copy .\lang\pt-BR.lng .\files

.\files\zh-CN.lng : .\lang\zh-CN.lng .\files\utils\SAVEreg.bat
	copy .\lang\zh-CN.lng .\files

.\files\zh-TW.lng : .\lang\zh-TW.lng .\files\utils\SAVEreg.bat
	copy .\lang\zh-TW.lng .\files

.\files\utils\RESTOREreg.bat : .\util\RESTOREreg.bat .\files\utils\SAVEreg.bat
	copy .\util\RESTOREreg.bat  .\files\utils

composition : \
 .\files\utils\SAVEreg.bat \
 .\files\default.chm \
 .\files\en.chm \
 .\files\ja-JP.chm \
 .\files\ko-KR.chm \
 .\files\pt-BR.chm \
 .\files\zh-CN.chm \
 .\files\zh-TW.chm \
 .\files\default.lng \
 .\files\en.lng \
 .\files\ja-JP.lng \
 .\files\ko-KR.lng \
 .\files\pt-BR.lng \
 .\files\zh-CN.lng \
 .\files\zh-TW.lng \
 .\files\utils\RESTOREreg.bat

#en-us
#ja-jp 
#ko-kr 
#pt-br 
#zh-cn 
#zh-tw
