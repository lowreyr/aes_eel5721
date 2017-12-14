// Copyright (C) 2013-2017 Altera Corporation, San Jose, California, USA. All rights reserved.
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to
// whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// This agreement shall be governed in all respects by the laws of the State of California and
// by the laws of the United States of America.

///////////////////////////////////////////////////////////////////////////////////
// This host program runs a "hello world" kernel. This kernel prints out a
// message for if the work-item index matches a kernel argument.
//
// Most of this host program code is the basic elements of a OpenCL host
// program, handling the initialization and cleanup of OpenCL objects. The
// host program also makes queries through the OpenCL API to get various
// properties of the device.
///////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include "CL/opencl.h"
#include "AOCLUtils/aocl_utils.h"

using namespace aocl_utils;

#define STRING_BUFFER_LEN 1024

// Runtime constants
// Used to define the work set over which this kernel will execute.
static const size_t work_group_size = 8;  // 8 threads in the demo workgroup
// Defines kernel argument value, which is the workitem ID that will
// execute a printf call
static const int thread_id_to_output = 2;

// OpenCL runtime configuration
static cl_platform_id platform = NULL;
static cl_device_id device = NULL;
static cl_context context = NULL;
static cl_command_queue queue = NULL;

static cl_command_queue qark0 = NULL;
static cl_command_queue qark1 = NULL;
static cl_command_queue qark2 = NULL;
static cl_command_queue qark3 = NULL;
static cl_command_queue qark4 = NULL;
static cl_command_queue qark5 = NULL;
static cl_command_queue qark6 = NULL;
static cl_command_queue qark7 = NULL;
static cl_command_queue qark8 = NULL;
static cl_command_queue qark9 = NULL;
static cl_command_queue qark10 = NULL;
static cl_kernel addRoundKey0 = NULL;
static cl_kernel addRoundKey1 = NULL;
static cl_kernel addRoundKey2 = NULL;
static cl_kernel addRoundKey3 = NULL;
static cl_kernel addRoundKey4 = NULL;
static cl_kernel addRoundKey5 = NULL;
static cl_kernel addRoundKey6 = NULL;
static cl_kernel addRoundKey7 = NULL;
static cl_kernel addRoundKey8 = NULL;
static cl_kernel addRoundKey9 = NULL;
static cl_kernel addRoundKey10 = NULL;
static cl_command_queue qbs0 = NULL;
static cl_command_queue qbs1 = NULL;
static cl_command_queue qbs2 = NULL;
static cl_command_queue qbs3 = NULL;
static cl_command_queue qbs4 = NULL;
static cl_command_queue qbs5 = NULL;
static cl_command_queue qbs6 = NULL;
static cl_command_queue qbs7 = NULL;
static cl_command_queue qbs8 = NULL;
static cl_command_queue qbs9 = NULL;
static cl_kernel byteSubstitution0 = NULL;
static cl_kernel byteSubstitution1 = NULL;
static cl_kernel byteSubstitution2 = NULL;
static cl_kernel byteSubstitution3 = NULL;
static cl_kernel byteSubstitution4 = NULL;
static cl_kernel byteSubstitution5 = NULL;
static cl_kernel byteSubstitution6 = NULL;
static cl_kernel byteSubstitution7 = NULL;
static cl_kernel byteSubstitution8 = NULL;
static cl_kernel byteSubstitution9 = NULL;
static cl_command_queue keyQueue = NULL;
static cl_kernel keyExpansion = NULL;
static cl_command_queue qmc0 = NULL;
static cl_command_queue qmc1 = NULL;
static cl_command_queue qmc2 = NULL;
static cl_command_queue qmc3 = NULL;
static cl_command_queue qmc4 = NULL;
static cl_command_queue qmc5 = NULL;
static cl_command_queue qmc6 = NULL;
static cl_command_queue qmc7 = NULL;
static cl_command_queue qmc8 = NULL;
static cl_kernel mixColumn0 = NULL;
static cl_kernel mixColumn1 = NULL;
static cl_kernel mixColumn2 = NULL;
static cl_kernel mixColumn3 = NULL;
static cl_kernel mixColumn4 = NULL;
static cl_kernel mixColumn5 = NULL;
static cl_kernel mixColumn6 = NULL;
static cl_kernel mixColumn7 = NULL;
static cl_kernel mixColumn8 = NULL;
static cl_command_queue qsr = NULL;
static cl_kernel shiftRows = NULL;
static cl_program program = NULL;
static cl_mem key_buffer = NULL;
static cl_mem in_buffer = NULL;
static cl_mem out_buffer = NULL;

// Function prototypes
bool init();
void cleanup();
static void device_info_ulong( cl_device_id device, cl_device_info param, const char* name);
static void device_info_uint( cl_device_id device, cl_device_info param, const char* name);
static void device_info_bool( cl_device_id device, cl_device_info param, const char* name);
static void device_info_string( cl_device_id device, cl_device_info param, const char* name);
static void display_device_info( cl_device_id device );

// Entry point.
int main(int argc, char *argv[]) {

  //uint8_t *output = (uint8_t *)malloc(sizeof(uint8_t)*16);
  uint8_t key[16]   = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
  //char input[16];
  

  FILE *fp;
  FILE *fp2;
  //FILE *infile
  FILE *outfile;
  size_t fileSize;
  char *input;
  char *output;
  int c;

//  fp2 = fopen("text.txt","w");

//  for (int i = 1; i < argc; i++)
//  {
    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        fprintf(stderr, "cat: can't open %s\n", argv[1]);
        //continue;
    }
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);
    input = (char*) malloc(fileSize); // + 1);
    output = (char*) malloc(fileSize);
    //input[fileSize] = '\0';
    fread(input, sizeof(char), fileSize, fp);
    fclose(fp);
//  }



  cl_int status;

  if(!init()) {
    return -1;
  }
  key_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint8_t) * 16, key, &status);
  in_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char) * fileSize, input, &status);
  out_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(char) * fileSize, NULL, &status);

  status = clSetKernelArg(addRoundKey0, 0, sizeof(cl_mem), &in_buffer);
  status = clSetKernelArg(addRoundKey0, 1, sizeof(cl_mem), &key_buffer);
  status = clSetKernelArg(addRoundKey10, 0, sizeof(cl_mem), &out_buffer);
  checkError(status, "Failed to set kernel arg 0");


  printf("\nKernel initialization is complete.\n");
  printf("Launching the kernel...\n\n");

  // Configure work set over which the kernel will execute
  size_t wgSize[3] = {work_group_size, 1, 1};
  size_t gSize[3] = {work_group_size, 1, 1};
  size_t size = 1;

  // for(int i = 0; i < 16; i++)
  // {
  //   input[i] = fgetc(fp);
  // }

  // Launch the kernel
  status = clEnqueueTask(keyQueue, keyExpansion, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark0, addRoundKey0, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs0, byteSubstitution0, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qmc0, mixColumn0, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark1, addRoundKey1, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs1, byteSubstitution1, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qmc1, mixColumn1, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark2, addRoundKey2, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs2, byteSubstitution2, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qmc2, mixColumn2, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark3, addRoundKey3, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs3, byteSubstitution3, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qmc3, mixColumn3, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark4, addRoundKey4, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs4, byteSubstitution4, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qmc4, mixColumn4, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark5, addRoundKey5, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs5, byteSubstitution5, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qmc5, mixColumn5, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark6, addRoundKey6, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs6, byteSubstitution6, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qmc6, mixColumn6, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark7, addRoundKey7, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs7, byteSubstitution7, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qmc7, mixColumn7, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark8, addRoundKey8, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs8, byteSubstitution8, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qmc8, mixColumn8, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark9, addRoundKey9, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qbs9, byteSubstitution9, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qsr, shiftRows, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueTask(qark10, addRoundKey10, 0, NULL, NULL);//, 1, NULL, &size, &size, 0, NULL, NULL);

  // int i = 0;
  // do {
  //     input[i] = fgetc(fp);
  //     if( feof(fp) )
  //     {
  //       for(int j = i; j < 16; j++)
  //       {
  //         input[j] = 0;
  //       }
  //       fputc(0, fp2);
  //       break ;
  //     }
  //     i = (i + 1)&0xF;
  //     if (i == 0)
  //     {
  //       for(int k = 0; k < 16; k++)
  //       {
  //         fputc(output[k], fp2);
  //       }
  //     };
  //  } while(1);


  checkError(status, "Failed to launch kernel");

  // Wait for command queue to complete pending events
  status = clFinish(qark10);
  checkError(status, "Failed to finish");

  // Read result
  status = clEnqueueReadBuffer(qark10, out_buffer, CL_TRUE, 0, sizeof(char) * fileSize, output, 0, NULL, NULL);
  
  outfile = fopen("text.txt","w");
  fwrite(output, sizeof(char), fileSize, outfile);
  fclose(outfile);


/*
  printf("\nAES output: ");
  for(int i=0; i<16; i++){
    printf("%x ",output[i]);
  }
  printf("\ninput: ");
  for(int i=0; i<16; i++){
    printf("%x ",input[i]);
  }
  printf("\nkey: ");
  for(int i=0; i<16; i++){
    printf("%x ",key[i]);
  }
*/

  printf("\nKernel execution is complete.\n");

  //  fclose(fp);
  //  fclose(fp2);

  // Free the resources allocated
  cleanup();

  return 0;
}

/////// HELPER FUNCTIONS ///////

bool init() {
  cl_int status;

  if(!setCwdToExeDir()) {
    return false;
  }

  // Get the OpenCL platform.
  platform = findPlatform("Intel");
  if(platform == NULL) {
    printf("ERROR: Unable to find Intel FPGA OpenCL platform.\n");
    return false;
  }

  // User-visible output - Platform information
  {
    char char_buffer[STRING_BUFFER_LEN];
    printf("Querying platform for info:\n");
    printf("==========================\n");
    clGetPlatformInfo(platform, CL_PLATFORM_NAME, STRING_BUFFER_LEN, char_buffer, NULL);
    printf("%-40s = %s\n", "CL_PLATFORM_NAME", char_buffer);
    clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, STRING_BUFFER_LEN, char_buffer, NULL);
    printf("%-40s = %s\n", "CL_PLATFORM_VENDOR ", char_buffer);
    clGetPlatformInfo(platform, CL_PLATFORM_VERSION, STRING_BUFFER_LEN, char_buffer, NULL);
    printf("%-40s = %s\n\n", "CL_PLATFORM_VERSION ", char_buffer);
  }

  // Query the available OpenCL devices.
  scoped_array<cl_device_id> devices;
  cl_uint num_devices;

  devices.reset(getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices));

  // We'll just use the first device.
  device = devices[0];

  // Display some device information.
  display_device_info(device);

  // Create the context.
  context = clCreateContext(NULL, 1, &device, &oclContextCallback, NULL, &status);
  checkError(status, "Failed to create context");

  // Create the command queue.
  //queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  keyQueue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark0 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs0 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qmc0 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark1 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs1 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qmc1 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark2 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs2 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qmc2 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark3 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs3 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qmc3 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark4 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs4 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qmc4 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark5 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs5 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qmc5 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark6 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs6 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qmc6 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark7 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs7 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qmc7 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark8 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs8 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qmc8 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark9 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qbs9 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qsr = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  qark10 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  checkError(status, "Failed to create command queue");

  // Create the program.
  std::string binary_file = getBoardBinaryFile("hello_aes", device);
  printf("Using AOCX: %s\n", binary_file.c_str());
  program = createProgramFromBinary(context, binary_file.c_str(), &device, 1);

  // Build the program that was just created.
  status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
  checkError(status, "Failed to build program");

  // Create the kernel - name passed in here must match kernel name in the
  // original CL file, that was compiled into an AOCX file using the AOC tool
  //const char *kernel_name = "hello_world";  // Kernel name, as defined in the CL file
  //kernel = clCreateKernel(program, kernel_name, &status);
  keyExpansion = clCreateKernel(program, "keyExpansion", &status);
  addRoundKey0 = clCreateKernel(program, "addRoundKey0", &status);
  byteSubstitution0 = clCreateKernel(program, "byteSubstitution0", &status);
  mixColumn0 = clCreateKernel(program, "mixColumn0", &status);
  addRoundKey1 = clCreateKernel(program, "addRoundKey1", &status);
  byteSubstitution1 = clCreateKernel(program, "byteSubstitution1", &status);
  mixColumn1 = clCreateKernel(program, "mixColumn1", &status);
  addRoundKey2 = clCreateKernel(program, "addRoundKey2", &status);
  byteSubstitution2 = clCreateKernel(program, "byteSubstitution2", &status);
  mixColumn2 = clCreateKernel(program, "mixColumn2", &status);
  addRoundKey3 = clCreateKernel(program, "addRoundKey3", &status);
  byteSubstitution3 = clCreateKernel(program, "byteSubstitution3", &status);
  mixColumn3 = clCreateKernel(program, "mixColumn3", &status);
  addRoundKey4 = clCreateKernel(program, "addRoundKey4", &status);
  byteSubstitution4 = clCreateKernel(program, "byteSubstitution4", &status);
  mixColumn4 = clCreateKernel(program, "mixColumn4", &status);
  addRoundKey5 = clCreateKernel(program, "addRoundKey5", &status);
  byteSubstitution5 = clCreateKernel(program, "byteSubstitution5", &status);
  mixColumn5 = clCreateKernel(program, "mixColumn5", &status);
  addRoundKey6 = clCreateKernel(program, "addRoundKey6", &status);
  byteSubstitution6 = clCreateKernel(program, "byteSubstitution6", &status);
  mixColumn6 = clCreateKernel(program, "mixColumn6", &status);
  addRoundKey7 = clCreateKernel(program, "addRoundKey7", &status);
  byteSubstitution7 = clCreateKernel(program, "byteSubstitution7", &status);
  mixColumn7 = clCreateKernel(program, "mixColumn7", &status);
  addRoundKey8 = clCreateKernel(program, "addRoundKey8", &status);
  byteSubstitution8 = clCreateKernel(program, "byteSubstitution8", &status);
  mixColumn8 = clCreateKernel(program, "mixColumn8", &status);
  addRoundKey9 = clCreateKernel(program, "addRoundKey9", &status);
  byteSubstitution9 = clCreateKernel(program, "byteSubstitution9", &status);
  shiftRows = clCreateKernel(program, "shiftRows", &status);
  addRoundKey10 = clCreateKernel(program, "addRoundKey10", &status);
  checkError(status, "Failed to create kernels");

  return true;
}

// Free the resources allocated during initialization
void cleanup() {
  if(keyExpansion) {
    clReleaseKernel(keyExpansion);
  }
  if(addRoundKey0) {
    clReleaseKernel(addRoundKey0);
  }
  if(byteSubstitution0) {
    clReleaseKernel(byteSubstitution0);
  }
  if(mixColumn0) {
    clReleaseKernel(mixColumn0);
  }
  if(addRoundKey1) {
    clReleaseKernel(addRoundKey1);
  }
  if(byteSubstitution1) {
    clReleaseKernel(byteSubstitution1);
  }
  if(mixColumn1) {
    clReleaseKernel(mixColumn1);
  }
  if(addRoundKey2) {
    clReleaseKernel(addRoundKey2);
  }
  if(byteSubstitution2) {
    clReleaseKernel(byteSubstitution2);
  }
  if(mixColumn2) {
    clReleaseKernel(mixColumn2);
  }
  if(addRoundKey3) {
    clReleaseKernel(addRoundKey3);
  }
  if(byteSubstitution3) {
    clReleaseKernel(byteSubstitution3);
  }
  if(mixColumn3) {
    clReleaseKernel(mixColumn3);
  }
  if(addRoundKey4) {
    clReleaseKernel(addRoundKey4);
  }
  if(byteSubstitution4) {
    clReleaseKernel(byteSubstitution4);
  }
  if(mixColumn4) {
    clReleaseKernel(mixColumn4);
  }
  if(addRoundKey5) {
    clReleaseKernel(addRoundKey5);
  }
  if(byteSubstitution5) {
    clReleaseKernel(byteSubstitution5);
  }
  if(mixColumn5) {
    clReleaseKernel(mixColumn5);
  }
  if(addRoundKey6) {
    clReleaseKernel(addRoundKey6);
  }
  if(byteSubstitution6) {
    clReleaseKernel(byteSubstitution6);
  }
  if(mixColumn6) {
    clReleaseKernel(mixColumn6);
  }
  if(addRoundKey7) {
    clReleaseKernel(addRoundKey7);
  }
  if(byteSubstitution7) {
    clReleaseKernel(byteSubstitution7);
  }
  if(mixColumn7) {
    clReleaseKernel(mixColumn7);
  }
  if(addRoundKey8) {
    clReleaseKernel(addRoundKey8);
  }
  if(byteSubstitution8) {
    clReleaseKernel(byteSubstitution8);
  }
  if(mixColumn8) {
    clReleaseKernel(mixColumn8);
  }
  if(addRoundKey9) {
    clReleaseKernel(addRoundKey9);
  }
  if(byteSubstitution9) {
    clReleaseKernel(byteSubstitution9);
  }
  if(shiftRows) {
    clReleaseKernel(shiftRows);
  }
  if(addRoundKey10) {
    clReleaseKernel(addRoundKey10);
  }
  if(program) {
    clReleaseProgram(program);
  }
  if(queue) {
    clReleaseCommandQueue(queue);
  }
  if(keyQueue) {
    clReleaseCommandQueue(keyQueue);
  }

  if (qark0) { clReleaseCommandQueue(qark0); }
  if (qbs0) { clReleaseCommandQueue(qbs0); }
  if (qmc0) { clReleaseCommandQueue(qmc0); }
  if (qark1) { clReleaseCommandQueue(qark1); }
  if (qbs1) { clReleaseCommandQueue(qbs1); }
  if (qmc1) { clReleaseCommandQueue(qmc1); }
  if (qark2) { clReleaseCommandQueue(qark2); }
  if (qbs2) { clReleaseCommandQueue(qbs2); }
  if (qmc2) { clReleaseCommandQueue(qmc2); }
  if (qark3) { clReleaseCommandQueue(qark3); }
  if (qbs3) { clReleaseCommandQueue(qbs3); }
  if (qmc3) { clReleaseCommandQueue(qmc3); }
  if (qark4) { clReleaseCommandQueue(qark4); }
  if (qbs4) { clReleaseCommandQueue(qbs4); }
  if (qmc4) { clReleaseCommandQueue(qmc4); }
  if (qark5) { clReleaseCommandQueue(qark5); }
  if (qbs5) { clReleaseCommandQueue(qbs5); }
  if (qmc5) { clReleaseCommandQueue(qmc5); }
  if (qark6) { clReleaseCommandQueue(qark6); }
  if (qbs6) { clReleaseCommandQueue(qbs6); }
  if (qmc6) { clReleaseCommandQueue(qmc6); }
  if (qark7) { clReleaseCommandQueue(qark7); }
  if (qbs7) { clReleaseCommandQueue(qbs7); }
  if (qmc7) { clReleaseCommandQueue(qmc7); }
  if (qark8) { clReleaseCommandQueue(qark8); }
  if (qbs8) { clReleaseCommandQueue(qbs8); }
  if (qmc8) { clReleaseCommandQueue(qmc8); }
  if (qark9) { clReleaseCommandQueue(qark9); }
  if (qbs9) { clReleaseCommandQueue(qbs9); }
  if (qsr) { clReleaseCommandQueue(qsr); }
  if (qark10) { clReleaseCommandQueue(qark10); }

  if(context) {
    clReleaseContext(context);
  }
}

// Helper functions to display parameters returned by OpenCL queries
static void device_info_ulong( cl_device_id device, cl_device_info param, const char* name) {
   cl_ulong a;
   clGetDeviceInfo(device, param, sizeof(cl_ulong), &a, NULL);
   printf("%-40s = %lu\n", name, a);
}
static void device_info_uint( cl_device_id device, cl_device_info param, const char* name) {
   cl_uint a;
   clGetDeviceInfo(device, param, sizeof(cl_uint), &a, NULL);
   printf("%-40s = %u\n", name, a);
}
static void device_info_bool( cl_device_id device, cl_device_info param, const char* name) {
   cl_bool a;
   clGetDeviceInfo(device, param, sizeof(cl_bool), &a, NULL);
   printf("%-40s = %s\n", name, (a?"true":"false"));
}
static void device_info_string( cl_device_id device, cl_device_info param, const char* name) {
   char a[STRING_BUFFER_LEN];
   clGetDeviceInfo(device, param, STRING_BUFFER_LEN, &a, NULL);
   printf("%-40s = %s\n", name, a);
}

// Query and display OpenCL information on device and runtime environment
static void display_device_info( cl_device_id device ) {

   printf("Querying device for info:\n");
   printf("========================\n");
   device_info_string(device, CL_DEVICE_NAME, "CL_DEVICE_NAME");
   device_info_string(device, CL_DEVICE_VENDOR, "CL_DEVICE_VENDOR");
   device_info_uint(device, CL_DEVICE_VENDOR_ID, "CL_DEVICE_VENDOR_ID");
   device_info_string(device, CL_DEVICE_VERSION, "CL_DEVICE_VERSION");
   device_info_string(device, CL_DRIVER_VERSION, "CL_DRIVER_VERSION");
   device_info_uint(device, CL_DEVICE_ADDRESS_BITS, "CL_DEVICE_ADDRESS_BITS");
   device_info_bool(device, CL_DEVICE_AVAILABLE, "CL_DEVICE_AVAILABLE");
   device_info_bool(device, CL_DEVICE_ENDIAN_LITTLE, "CL_DEVICE_ENDIAN_LITTLE");
   device_info_ulong(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, "CL_DEVICE_GLOBAL_MEM_CACHE_SIZE");
   device_info_ulong(device, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE");
   device_info_ulong(device, CL_DEVICE_GLOBAL_MEM_SIZE, "CL_DEVICE_GLOBAL_MEM_SIZE");
   device_info_bool(device, CL_DEVICE_IMAGE_SUPPORT, "CL_DEVICE_IMAGE_SUPPORT");
   device_info_ulong(device, CL_DEVICE_LOCAL_MEM_SIZE, "CL_DEVICE_LOCAL_MEM_SIZE");
   device_info_ulong(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, "CL_DEVICE_MAX_CLOCK_FREQUENCY");
   device_info_ulong(device, CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_MAX_COMPUTE_UNITS");
   device_info_ulong(device, CL_DEVICE_MAX_CONSTANT_ARGS, "CL_DEVICE_MAX_CONSTANT_ARGS");
   device_info_ulong(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE");
   device_info_uint(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS");
   device_info_uint(device, CL_DEVICE_MEM_BASE_ADDR_ALIGN, "CL_DEVICE_MEM_BASE_ADDR_ALIGN");
   device_info_uint(device, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, "CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE");
   device_info_uint(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR");
   device_info_uint(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT");
   device_info_uint(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT");
   device_info_uint(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG");
   device_info_uint(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT");
   device_info_uint(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE");

   {
      cl_command_queue_properties ccp;
      clGetDeviceInfo(device, CL_DEVICE_QUEUE_PROPERTIES, sizeof(cl_command_queue_properties), &ccp, NULL);
      printf("%-40s = %s\n", "Command queue out of order? ", ((ccp & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)?"true":"false"));
      printf("%-40s = %s\n", "Command queue profiling enabled? ", ((ccp & CL_QUEUE_PROFILING_ENABLE)?"true":"false"));
   }
}