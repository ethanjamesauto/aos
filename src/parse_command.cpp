#include "parse_command.h"
#include "mandelbrot.h"

struct vbe_info_structure {
    char signature[4];            // must be "VESA" to indicate valid VBE support
    unsigned short version;       // VBE version; high byte is major version, low byte is minor version
    unsigned oem;                 // segment:offset pointer to OEM
    unsigned capabilities;        // bitfield that describes card capabilities
    unsigned video_modes;         // segment:offset pointer to list of supported video modes
    unsigned short video_memory;  // amount of video memory in 64KB blocks
    unsigned short software_rev;  // software revision
    unsigned vendor;              // segment:offset to card vendor string
    unsigned product_name;        // segment:offset to card model name
    unsigned product_rev;         // segment:offset pointer to product revision
    char reserved[222];           // reserved for future expansion
    char oem_data[256];           // OEM BIOSes store their strings in this area
} __attribute__((packed));

unsigned short* modes;

void print_vbe_info(vbe_info_structure& vbe, Writer& writer) {
    writer << "Signature: " << vbe.signature << endl;
    writer << "Version: " << (vbe.version >> 8) << "." << (vbe.version & 0xff) << endl;
    writer << "Video Memory Size: " << vbe.video_memory << "KB" << endl;
    writer << "Software Revision: " << vbe.software_rev << endl;
    vbe.vendor = ((vbe.vendor >> 16) << 4) + (vbe.vendor & 0xffff);
    vbe.product_name = ((vbe.product_name >> 16) << 4) + (vbe.product_name & 0xffff);
    vbe.product_rev = ((vbe.product_rev >> 16) << 4) + (vbe.product_rev & 0xffff);
    vbe.video_modes = ((vbe.video_modes >> 16) << 4) + (vbe.video_modes & 0xffff);
    if (vbe.vendor != 0) {
        writer << "Vendor: " << (char*)vbe.vendor << endl;
    }
    if (vbe.product_name != 0) {
        writer << "Product Name: " << (char*)vbe.product_name << endl;
    }
    if (vbe.product_rev != 0) {
        writer << "Product Revision: " << (char*)vbe.product_rev << endl;
    }
    modes = (unsigned short*)vbe.video_modes;
    writer << "Mode List: ";
    for (int i = 0; modes[i] != 0xffff; i++) {
        writer.printIntBase(modes[i], 16, 4);
        writer << " ";
    }
    writer << endl;
}

bool equals(const char* a, const char* b) {
    for (int i = 0; a[i] == b[i]; i++) {
        if (a[i] == '\0') {
            return true;
        }
    }
    return false;
}

bool parse_command(char* command, Writer& writer) {
    if (equals(command, "ethan is awesome")) {
        writer << "he is!!!" << endl;
        return true;
    } else if (equals(command, "vesainfo")) {
        vbe_info_structure vesa = *(vbe_info_structure*)0x500;
        print_vbe_info(vesa, writer);
        return true;
    } else if (equals(command, "hello")) {
        writer << "Hello! How are you?" << endl;
        return true;
    } else if (equals(command, "clear")) {
        writer.clearScreen();
        return true;
    } else if (equals(command, "mb")) {
        runMandelbrot(writer, WIDTH, HEIGHT - 1);
        return true;
    } else if (equals(command, "poop")) {
        writer << (int)(1.0 * 361. * 21. / 5.) << endl;
        return true;
    }
    return false;
}
