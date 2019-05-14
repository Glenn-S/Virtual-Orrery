################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Camera.cpp \
../Geometry.cpp \
../Program.cpp \
../RenderingEngine.cpp \
../Scene.cpp \
../ShaderTools.cpp \
../TextureParser.cpp \
../main.cpp \
../texture.cpp 

C_SRCS += \
../glad.c 

OBJS += \
./Camera.o \
./Geometry.o \
./Program.o \
./RenderingEngine.o \
./Scene.o \
./ShaderTools.o \
./TextureParser.o \
./glad.o \
./main.o \
./texture.o 

CPP_DEPS += \
./Camera.d \
./Geometry.d \
./Program.d \
./RenderingEngine.d \
./Scene.d \
./ShaderTools.d \
./TextureParser.d \
./main.d \
./texture.d 

C_DEPS += \
./glad.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/rogue1/eclipse-workspace/CPSC_453_Ass5/middleware/glad/include" -I"/home/rogue1/eclipse-workspace/CPSC_453_Ass5/middleware/glfw/include" -I"/home/rogue1/eclipse-workspace/CPSC_453_Ass5/middleware/glm-0.9.8.2" -I"/home/rogue1/eclipse-workspace/CPSC_453_Ass5/middleware/stb" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/rogue1/eclipse-workspace/CPSC_453_Ass5/middleware/glad/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


