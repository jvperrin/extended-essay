#include <iostream>
#include <fstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

int main () {
  const int BLOCK_SIZE = 512; // Number of bytes per logical sector, `sudo fdisk -l` shows the sector size (logical / physical).
  const int NUM_BLOCKS = 374859776; // Number of blocks of size BLOCKSIZE in the device.
  const int BUFFER_SIZE = 1000000; // Size of the buffer used for reading the data.
  const char* filename = "/dev/sda6";

  char buffer[BUFFER_SIZE];

  ifstream device (filename, ios::in | ios::binary); // ifstream is for read-only access, fstream is for read/write.

  if (device.is_open()) {
    cout << "Opened!" << endl;

    device.seekg (0, device.end);
    cout << "Length of device: " << device.tellg() << " bytes" << endl;
  	device.seekg (0, device.beg);

    for(int i = 0; i < 100; i++) {
    // for(int i = 0; i < NUMBLOCKS; i++) {
      cout << i << endl;
      device.read(buffer, BUFFER_SIZE);
      cout.write(buffer, BUFFER_SIZE);
      // usleep(3000);
    }

    device.close();
  } else {
    cout << "Unable to open /dev/sda6." << endl;
  }

  return 0;
}
