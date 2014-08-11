#include <iostream>
#include <fstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

int main () {
  const int BLOCK_SIZE = 512; // Number of bytes per logical sector, `sudo fdisk -l` shows the sector size (logical / physical).
  const int NUM_BYTES = 2146435072; // Number of bytes in the device.
  const int BUFFER_SIZE = 1048576 * 2; // Size of the buffer used for reading the data.
  // 1MB: 1048576 timing
  // real	22m33.236s
  // user	0m0.100s
  // sys	0m39.942s
  // 16MB: 16769024 doesn't run?
  const char* filename = "/dev/sda7";

  char buffer[BUFFER_SIZE];

  ifstream device (filename, ios::in | ios::binary); // ifstream is for read-only access, fstream is for read/write.

  if (device.is_open()) {
    cout << "Opened!" << endl;

    // device.seekg (0, device.end);
    // cout << "Length of device: " << device.tellg() << " bytes" << endl;
    // device.seekg (0, device.beg);

    for(int i = 0; i < (NUM_BYTES/BUFFER_SIZE); i++) {
      cout << i << endl;
      device.read(buffer, BUFFER_SIZE);
      cout.write(buffer, BUFFER_SIZE);
    }

    device.close();
  } else {
    cout << "Unable to open " << filename << endl;
  }

  return 0;
}
