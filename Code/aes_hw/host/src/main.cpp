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
static cl_command_queue keyQueue = NULL;
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
static cl_kernel keyExpansion = NULL;
static cl_kernel mixColumn0 = NULL;
static cl_kernel mixColumn1 = NULL;
static cl_kernel mixColumn2 = NULL;
static cl_kernel mixColumn3 = NULL;
static cl_kernel mixColumn4 = NULL;
static cl_kernel mixColumn5 = NULL;
static cl_kernel mixColumn6 = NULL;
static cl_kernel mixColumn7 = NULL;
static cl_kernel mixColumn8 = NULL;
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
int main() {

  uint8_t *output = (uint8_t *)malloc(sizeof(uint8_t)*16);
  uint8_t key[16]   = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
  uint8_t input[16];

  cl_int status;

  if(!init()) {
    return -1;
  }
  key_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint8_t) * 16, key, &status);
  in_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint8_t) * 16, input, &status);
  out_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(uint8_t) * 16, NULL, &status);

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

  FILE *fp;
  FILE *fp2;
  int c;

  fp = fopen("hello.txt", "r");
  fp2 = fopen("text.txt","w");
  fseek(fp,0,SEEK_END);
  size_t file_size = ftell(fp);
  rewind(fp);

  for(int i = 0; i < 16; i++)
  {
    if( feof(fp) )
    {
      input[i] = 0;
    }
    else
    {
      input[i] = fgetc(fp);
    }
    printf("%c",input[i]);
  }

  const double start_time = getCurrentTimestamp();
  // Launch the kernel
  status = clEnqueueNDRangeKernel(keyQueue, keyExpansion, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey0, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution0, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, mixColumn0, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey1, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution1, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, mixColumn1, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey2, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution2, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, mixColumn2, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey3, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution3, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, mixColumn3, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey4, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution4, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, mixColumn4, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey5, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution5, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, mixColumn5, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey6, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution6, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, mixColumn6, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey7, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution7, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, mixColumn7, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey8, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution8, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, mixColumn8, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey9, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, byteSubstitution9, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, shiftRows, 1, NULL, &size, &size, 0, NULL, NULL);
  status = clEnqueueNDRangeKernel(queue, addRoundKey10, 1, NULL, &size, &size, 0, NULL, NULL);

  checkError(status, "Failed to launch kernel");

  // Wait for command queue to complete pending events
  status = clFinish(queue);
  checkError(status, "Failed to finish");

  const double end_time = getCurrentTimestamp();

  // Wall-clock time taken.
  printf("\nTime: %0.3f ms\n", (end_time - start_time) * 1e3);

  // Read result
  status = clEnqueueReadBuffer(queue, out_buffer, CL_TRUE, 0, sizeof(uint8_t) * file_size, output, 0, NULL, NULL);

  for(int i = 0; i < 16; i++)
  {
    fputc(output[i], fp2);
  }

  fclose(fp);
  fclose(fp2);

  printf("\nAES output: ");
  for(int i=0; i<16; i++){
    printf("%x",output[i]);
  }
  printf("\ninput: ");
  for(int i=0; i<16; i++){
    printf("%x",input[i]);
  }
  printf("\nkey: ");
  for(int i=0; i<16; i++){
    printf("%x",key[i]);
  }


  printf("\nKernel execution is complete.\n");

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
  queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  keyQueue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  checkError(status, "Failed to create command queue");

  // Create the program.
  std::string binary_file = getBoardBinaryFile("aes", device);
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
