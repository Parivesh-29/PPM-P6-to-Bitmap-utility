#include <stdio.h>

int main() {


	char infileName[50];  //source file path
	char p6[10];
	int w = 0;		// width of the image
	int h = 0;		// height of the image
	int max = 0;	// maximum pixel value

    printf("specify address of input file:");
    scanf("%s",&infileName);

    // open the file to read just the header reading
	FILE* fr = fopen(infileName, "rb");

	//  reading header
	fscanf(fr, "%s", p6);

	// check to see if it's a PPM image file
	if (strncmp(p6, "P6" , 10) != 0) {
		printf("invalid file format or invalid path \n");
		return 0;
	}

	// reading info header
	fscanf(fr, "%d\n %d\n", &w, &h);
    fscanf(fr, "%d\n", &max);

    int size = (w) * (h) * 3; //ppm file size in bytes

     unsigned char data[size]; //linear array to store pixel data
     unsigned char *p = &data;  // pointer to array

		for(int k=0;k<size;k=k+3){    //reading pixel data and storing it in B,G,R order
            for(int z=0;z<3;z++){
                fread((p+k+2-z), 1, 1, fr);
		    }
		}

    // close file
	fclose(fr);



  //writing a bmp file
    char outfileName[50];
    printf("specify name and directory for output file with .bmp extension:");
    scanf("%s",&outfileName);
	FILE *f;

    int filesize = 54 + size + ((4-((3*w)%4))* h);  //filesize of bmp file in bytes

    unsigned char fileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};  // describing bitmap file header
    unsigned char infoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};  // describing bitmap info header
    unsigned char pad[3] = {0,0,0}; //data for padding

    fileheader[ 2] = (unsigned char)(filesize);
    fileheader[ 3] = (unsigned char)(filesize>> 8);
    fileheader[ 4] = (unsigned char)(filesize>>16);
    fileheader[ 5] = (unsigned char)(filesize>>24);

    infoheader[ 4] = (unsigned char)(w);
    infoheader[ 5] = (unsigned char)(w>> 8);
    infoheader[ 6] = (unsigned char)(w>>16);
    infoheader[ 7] = (unsigned char)(w>>24);
    infoheader[ 8] = (unsigned char)(h);
    infoheader[ 9] = (unsigned char)(h>> 8);
    infoheader[10] = (unsigned char)(h>>16);
    infoheader[11] = (unsigned char)(h>>24);

    f = fopen(outfileName,"wb");  //name and path of output bmp file
    fwrite(fileheader,1,14,f); // writing file header
    fwrite(infoheader,1,40,f); // writing info header

    for(int i=0; i<h; i++)    // writing pixel data from bottom to top row in B,G,R order
    {
        fwrite((p+ (w *(h-1-i)*3)),3,w,f);
       fwrite(pad,1,(4-(w *3)%4)%4,f);
    }


    fclose(f); //close file

    printf("bitmap image file created");
        return 0;
}
