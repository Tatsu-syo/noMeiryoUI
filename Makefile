ROOT_DIR = .

.IGNORE:

help : help\*
	cd .\help\en-us
	hhc help_en.hhp
	cd ..\..

	cd .\help\ja-jp
	hhc .\help_ja-jp.hhp
	cd ..\..

	cd .\help\ko-kr
	hhc help_ko-kr.hhp
	cd ..\..

	cd .\help\pt-br
	hhc help_br.hhp
	cd ..\..

	cd .\help\zh-cn
	hhc help_zh-cn.hhp
	cd ..\..

	cd .\help\zh-tw
	hhc help_zh-tw.hhp
	cd ..\..

!CMDSWITCHES

#en-us
#ja-jp 
#ko-kr 
#pt-br 
#zh-cn 
#zh-tw
