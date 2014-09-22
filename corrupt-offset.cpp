#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

using namespace std;

// Mount filesystem:
//   sudo umount /dev/sda7
// Unmount filesystem:
//   sudo mount /dev/sda7
// Check/repair filesystem:
//   sudo fsck -t ext4 /dev/sda7

// Show offset in decimal:
//   strings --radix=d /dev/sda8

int main (int argc, char* argv[]) {
  const char* COMPARISON_STRING = "But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was born and I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes, or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful. Nor again is there anyone who loves or pursues or d";
  const unsigned int BUFFER_SIZE = 512; // Size of the buffer used for reading the data.
                                        //   Equal to the number of bytes in a logical sector.
                                        //   `sudo fdisk -l` shows the sector size (logical / physical).
  char buffer[BUFFER_SIZE];
  unsigned long long offset;
  fstream device (argv[1], ios::in | ios::out | ios::binary);

  if (argc < 3) {
    cout << "Usage: " << argv[0] << " <file system> <offset>" << endl;
    return 1;
  }

  offset = strtoull(argv[2], NULL, 10);

  if (device.is_open()) {
    cout << "Opened!" << endl;

    cout << "Seeking to " << offset << endl;

    device.clear(); // Clears any fail bits (eof) before seeking again.
    if (device.seekg(offset, ios::beg).rdstate() == 0) {
      if (device.read(buffer, BUFFER_SIZE).rdstate() == 0) {
        cout << "Existing data: " << buffer << endl;

        sleep(5); // Give time to abort the overwrite

        device.seekg(offset, ios::beg); // Seek to offset from beginning of device
        memset(&buffer[0], 0, BUFFER_SIZE); // Clear data in buffer
        strcpy(buffer, "This is the overwritten data"); // Copy string to the buffer

        if (device.write(buffer, BUFFER_SIZE).rdstate() == 0) {
          cout << "Overwritten successfully!" << endl;
        } else {
          cout << "Write error: " << errno << endl;
          return 2;
        }
      } else {
        cout << "Read error" << endl;
        return 3;
      }
    } else {
      cout << "Seek error" << endl;;
      return 4;
    }

    device.close();
  } else {
    cout << "Unable to open " << argv[1] << endl;
    return 5;
  }

  return 0;
}
