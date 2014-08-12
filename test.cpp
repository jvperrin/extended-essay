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
  unsigned long long offset = 0;

  fstream device (filename, ios::in | ios::out | ios::binary); // fstream is for read/write.

  if (device.is_open()) {
    cout << "Opened!" << endl;

    // device.seekg (0, device.end);
    // cout << "Length of device: " << device.tellg() << " bytes" << endl;
    // device.seekg (0, device.beg);

    for(int i = 0; i < (NUM_BYTES/BUFFER_SIZE); i++) {
      device.read(buffer, BUFFER_SIZE);

      if (strcmp(buffer, "This is a full test this time, let's see if it works\n") == 0) {
        offset = device.tellg();
        offset = offset - BUFFER_SIZE;
        cout << "Match at offset " << offset << endl;
      }
    }

    if (offset != 0) {
      cout << "Seeking to " << offset << endl;

      device.clear(); // Clears any fail bits (eof) before seeking again.
      if (device.seekg(offset, ios::beg).rdstate() == 0) {
        if (device.read(buffer, BUFFER_SIZE).rdstate() == 0) {
          cout << buffer << endl;
          device.seekg(offset, ios::beg);
          memset(&buffer[0], 0, BUFFER_SIZE);
          strcpy(buffer, "This is the overwritten data");
          cout << device.write(buffer, BUFFER_SIZE).rdstate() << endl;
        } else {
          cout << "Read error\n";
        }
      } else {
        cout << "Seek error\n";
      }
      // sudo fsck -t ext4 /dev/sda7
    } else {
      cout << "No matches found!\n";
    }

    device.close();
  } else {
    cout << "Unable to open " << filename << endl;
  }

  return 0;
}
