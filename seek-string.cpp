#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

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
  // const char* filename = "/dev/sda8";

  char buffer[BUFFER_SIZE];
  unsigned long long offset = 0;

  fstream device (argv[1], ios::in | ios::out | ios::binary);

  if (device.is_open()) {
    cout << "Opened!" << endl;

    device.seekg (0, device.end);
    const unsigned long long NUM_BYTES = device.tellg(); // Number of bytes in the device.
    cout << "Length of device: " << NUM_BYTES << " bytes" << endl;
    device.seekg (0, device.beg);

    for(int i = 0; i < (NUM_BYTES/BUFFER_SIZE); i++) {
      device.read(buffer, BUFFER_SIZE);

      if (strncmp(buffer, COMPARISON_STRING, 512) == 0) {
        offset = device.tellg();
        offset = offset - BUFFER_SIZE;
        cout << "Match at offset " << offset << endl;
      }
    }

    device.close();
  } else {
    cout << "Unable to open " << argv[1] << endl;
  }

  return 0;
}
