#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

int main () {
  const unsigned int BLOCK_SIZE = 512; // Number of bytes per logical sector, `sudo fdisk -l` shows the sector size (logical / physical).
  const unsigned long long NUM_BYTES = 2146435072; // Number of bytes in the device.
  const unsigned int BUFFER_SIZE = 512; // Size of the buffer used for reading the data.
  // 1MB: 1048576 timing => 34s without output
  const char* filename = "/dev/sda7";

  char buffer[BUFFER_SIZE];
  unsigned long long offset;

  ifstream device (filename, ios::in | ios::binary); // ifstream is for read-only access, fstream is for read/write.

  if (device.is_open()) {
    cout << "Opened!" << endl;

    // device.seekg (0, device.end);
    // cout << "Length of device: " << device.tellg() << " bytes" << endl;
    // device.seekg (0, device.beg);

    for(int i = 0; i < (NUM_BYTES/BUFFER_SIZE); i++) {
      device.read(buffer, BUFFER_SIZE);

      if (strcmp(buffer, "This is a full test this time, just to see what happens\n") == 0) {
        offset = device.tellg();
        cout << "Match at offset " << offset << endl;
      }
    }

    cout << "Seeking to " << offset << endl;
    if (device.seekg(fpos(offset)) == 0) {
      if (device.read(buffer, 512) == 0) {
        cout << buffer << endl;
      } else {
        cout << "Read error\n";
      }
    } else {
      cout << "Seek error\n";
    }
    // sudo fsck -t ext4 /dev/sda7

    device.close();
  } else {
    cout << "Unable to open " << filename << endl;
  }

  return 0;
}
