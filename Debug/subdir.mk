################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Config.cpp \
<<<<<<< .mine
../LogHelper.cpp 

||||||| .r1026
../LogHelper.cpp 

=======
../FileWriter.cpp \
../LogHelper.cpp \
../RdkafkaConsumer.cpp \
../RdkafkaProducer.cpp \
../Util.cpp 

>>>>>>> .r1102
OBJS += \
./Config.o \
<<<<<<< .mine
./LogHelper.o 

||||||| .r1026
./LogHelper.o 

=======
./FileWriter.o \
./LogHelper.o \
./RdkafkaConsumer.o \
./RdkafkaProducer.o \
./Util.o 

>>>>>>> .r1102
CPP_DEPS += \
./Config.d \
<<<<<<< .mine
./LogHelper.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Include -I../../muduo -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


||||||| .r1026
./LogHelper.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Include -I../../muduo -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


=======
./FileWriter.d \
./LogHelper.d \
./RdkafkaConsumer.d \
./RdkafkaProducer.d \
./Util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Include -I../../muduo -I../../kafka -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


>>>>>>> .r1102
