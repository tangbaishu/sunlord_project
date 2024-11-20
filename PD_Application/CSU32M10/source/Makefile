###################################################
#工具链定义处
###################################################
TOOLCHAIN_DIR = D:\Chipsea\CSU-IDE\chipsea\tools\Compiler\C
ASM	   				= "$(TOOLCHAIN_DIR)\csasm.exe"
CC    				= "$(TOOLCHAIN_DIR)\cscc.exe"
LINKER 				= "$(TOOLCHAIN_DIR)\cslinker.exe"
LIBMAKER 			= "$(TOOLCHAIN_DIR)\CSLibMaker.exe"
RM	   				= del /F 1>NUL 2>NUL



###################################################
#编译选项定义处
###################################################
OUTPUT_DIR            = D:\Work_Area\Software\PD_Application\CSU32M10\source\.\Release
INTERTMP_DIR          = D:\Work_Area\Software\PD_Application\CSU32M10\source\.\Release
PROJ_PATH			  = D:\Work_Area\Software\PD_Application\CSU32M10\source
PROJ_NAME			  = csu32m10_gpio
BODY_NAME			  = CSU32M10
CFLAGS                = -cc1 -S  -debug-info-kind=limited -O0 -triple=cscc -fno-inline -chip=CSU32M10 -I "./sample/inc" -I "./include"
ASMFLAGS              = -filetype=obj -arch=cscc -fill=0xffff -DM=__CHIPSEA__CSU32M10 -chip=CSU32M10 -mattr=c16 -BankCount=1
LINKFLAGS             = -bt 16
LIBMAKERFLAGS         = -chiptype 16 -debuginfo RemoveDebugInfo -encodeinfo Encryption
CODE_OPTION			  = %%CodeOption%%
ASM_FILES			  = "$(INTERTMP_DIR)\csu32m10_gpio.asm" \
"$(INTERTMP_DIR)\csu32m10_timer0.asm" \
"$(INTERTMP_DIR)\main.asm" \
"$(INTERTMP_DIR)\SysRegDefine.asm" \
"D:\Work_Area\Software\PD_Application\CSU32M10\source\cstartup.asm"

OBJ_FILES			  = "$(INTERTMP_DIR)\csu32m10_gpio.obj" \
"$(INTERTMP_DIR)\csu32m10_timer0.obj" \
"$(INTERTMP_DIR)\main.obj" \
"$(INTERTMP_DIR)\SysRegDefine.obj" \
"$(INTERTMP_DIR)\cstartup.obj"

OUTPUT_FILE_TYPE	  = binfile
OUTPUT_FILE_EXTENSION = hex
OUTPUT_OTHER_FILES	  = "$(INTERTMP_DIR)\*.asm" \
						    "$(INTERTMP_DIR)\*.obj" \
                            "$(INTERTMP_DIR)\*.map"	\
							"$(INTERTMP_DIR)\*.txt"	\
						    "$(INTERTMP_DIR)\*.xml" \
							"$(INTERTMP_DIR)\*.sbms" \
							"$(INTERTMP_DIR)\*.cscc_bk"



###################################################
#.C文件到.ASM文件
###################################################
"$(INTERTMP_DIR)\csu32m10_gpio.asm" : "D:\Work_Area\Software\PD_Application\CSU32M10\source\sample\src\csu32m10_gpio.c"
	$(CC) $(CFLAGS)  "D:\Work_Area\Software\PD_Application\CSU32M10\source\sample\src\csu32m10_gpio.c" -o "$(INTERTMP_DIR)\csu32m10_gpio.asm" 
	$(ECHO)
"$(INTERTMP_DIR)\csu32m10_timer0.asm" : "D:\Work_Area\Software\PD_Application\CSU32M10\source\sample\src\csu32m10_timer0.c"
	$(CC) $(CFLAGS)  "D:\Work_Area\Software\PD_Application\CSU32M10\source\sample\src\csu32m10_timer0.c" -o "$(INTERTMP_DIR)\csu32m10_timer0.asm" 
	$(ECHO)
"$(INTERTMP_DIR)\main.asm" : "D:\Work_Area\Software\PD_Application\CSU32M10\source\sample\src\main.c"
	$(CC) $(CFLAGS)  "D:\Work_Area\Software\PD_Application\CSU32M10\source\sample\src\main.c" -o "$(INTERTMP_DIR)\main.asm" 
	$(ECHO)
"$(INTERTMP_DIR)\SysRegDefine.asm" : "D:\Work_Area\Software\PD_Application\CSU32M10\source\sample\src\SysRegDefine.c"
	$(CC) $(CFLAGS)  "D:\Work_Area\Software\PD_Application\CSU32M10\source\sample\src\SysRegDefine.c" -o "$(INTERTMP_DIR)\SysRegDefine.asm" 
	$(ECHO)

#main.asm :
#	$(CC) $(CFLAGS) "$(PROJ_PATH)\main.c" -o "$(PROJ_PATH)\main.asm"

###################################################
#.ASM文件到.OBJ文件
###################################################
"$(INTERTMP_DIR)\csu32m10_gpio.obj" : "$(INTERTMP_DIR)\csu32m10_gpio.asm"
	$(ASM) $(ASMFLAGS)  "$(INTERTMP_DIR)\csu32m10_gpio.asm" -o "$(INTERTMP_DIR)\csu32m10_gpio.obj" 
	$(ECHO)
"$(INTERTMP_DIR)\csu32m10_timer0.obj" : "$(INTERTMP_DIR)\csu32m10_timer0.asm"
	$(ASM) $(ASMFLAGS)  "$(INTERTMP_DIR)\csu32m10_timer0.asm" -o "$(INTERTMP_DIR)\csu32m10_timer0.obj" 
	$(ECHO)
"$(INTERTMP_DIR)\main.obj" : "$(INTERTMP_DIR)\main.asm"
	$(ASM) $(ASMFLAGS)  "$(INTERTMP_DIR)\main.asm" -o "$(INTERTMP_DIR)\main.obj" 
	$(ECHO)
"$(INTERTMP_DIR)\SysRegDefine.obj" : "$(INTERTMP_DIR)\SysRegDefine.asm"
	$(ASM) $(ASMFLAGS)  "$(INTERTMP_DIR)\SysRegDefine.asm" -o "$(INTERTMP_DIR)\SysRegDefine.obj" 
	$(ECHO)
"$(INTERTMP_DIR)\cstartup.obj" : "D:\Work_Area\Software\PD_Application\CSU32M10\source\cstartup.asm"
	$(ASM) $(ASMFLAGS) -gstab "D:\Work_Area\Software\PD_Application\CSU32M10\source\cstartup.asm" -o "$(INTERTMP_DIR)\cstartup.obj" 
	$(ECHO)

#main.obj :
#	$(ASM) $(ASMFLAGS) "$(PROJ_PATH)\main.asm" -o "$(PROJ_PATH)\main.obj"

###################################################
#命令定义处
###################################################
.PHONY : build
prepare : prebuild outputdir

re_prepare : prebuild outputdir clean_buildfile

build  : prebuild outputdir compiler_all_c_files compiler_all_asm_files $(OUTPUT_FILE_TYPE) postbuild

.PHONY : rebuild
rebuild: prebuild outputdir clean_buildfile build postbuild

output_file : $(OUTPUT_FILE_TYPE) postbuild

.PHONY : clean
clean :
	$(RM)  "$(INTERTMP_DIR)\dee"
	$(RM)  "$(INTERTMP_DIR)\*.lik"
	$(RM)  "$(INTERTMP_DIR)\*.lst"
	$(RM)  $(OUTPUT_OTHER_FILES)
	$(RM)  "$(OUTPUT_DIR)\$(PROJ_NAME).$(OUTPUT_FILE_EXTENSION)"
	$(RM)  "$(INTERTMP_DIR)\$(PROJ_NAME).dat"

clean_buildfile :
	$(RM)  $(OUTPUT_OTHER_FILES)
	$(RM)  "$(OUTPUT_DIR)\$(PROJ_NAME).$(OUTPUT_FILE_EXTENSION)"
    
outputdir:
	if not exist "$(OUTPUT_DIR)/$(NULL)" mkdir "$(OUTPUT_DIR)"
	if not exist "$(INTERTMP_DIR)/$(NULL)" mkdir "$(INTERTMP_DIR)"
	
binfile: 
	$(LINKER) $(LINKFLAGS) -s "$(INTERTMP_DIR)\Script.lik"
	
libfile: 
	$(LIBMAKER) $(LIBMAKERFLAGS) -o $(OBJ_FILES) -lib "$(OUTPUT_DIR)\$(PROJ_NAME).lib"
	
compiler_all_c_files : $(ASM_FILES)

compiler_all_asm_files: $(OBJ_FILES)

prebuild :
	

postbuild :
	

