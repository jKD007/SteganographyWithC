#include "Stegano_header.h"

int main(int argc,char *argv[])
{
    int i;
    char *argv_2,*argv_3,*argv_4;
    // kiem tra so luong tham so
    switch(argc)
    {
	// neu la 8 thi xu li encoding
	case 8 :	if(!strcmp(argv[1],"-E"))// Kiem tra neu tham so thu 2 la -E thi xu li
			{
			    for (i = 2; i < 8; i++) // Duyet tu tham so thu 3 neu truoc no la -i thi no se luu argv_2 = Input Source BMP Image sau -i
			    {
				if (!strcmp(argv[i],"-i"))//				      -s thi no se luu argv_3 = Secret Text File sau -s
				    argv_2 = argv[i+1];	  // 				      -o thi no se luu argv_4 = Output BMP Image sau -o

				if (!strcmp(argv[i],"-s"))
				    argv_3 = argv[i+1];

				if (!strcmp(argv[i],"-o"))
				    argv_4 = argv[i+1];
			    }			    
				// Sau khi kiem tra co cac option thi du cac tham so de goi ham Encoding
			    Encoding(argv_2, argv_3, argv_4);
			    break;
			}
	// neu la 6 thi xu li decode
	case 6 :	if(!strcmp(argv[1], "-D")) // Kiem tra neu tham so thu 2 la -D thi xu li
			{
			    for (i = 2; i < 6; i++) // Duyet tu tham so thu 3 neu truoc no la -i thi no se luu argv_2 = Output Image sau -i
			    {
				if (!strcmp(argv[i],"-i"))// 				     -o thi no se luu argv_4 = Output Text File
				    argv_2 = argv[i+1];
				if (!strcmp(argv[i],"-o"))
				    argv_4 = argv[i+1];
			    }			    
				// Sau khi kiem tra co cac option thi du cac tham so de goi ham Encoding
			    Decode(argv_2, argv_4);
			    break;
			}
	// neu thieu tham so hoac ko dung tham so thu 1 thi no se toi case default.
	default : 
		printf("\t*** !! Error !! ERROR !! Error !!***\n\n");
		printf("*** EXECUTION ==> ");
		printf("\n\t\t*** ENCODING *** ==> \t./a.out -E -i <input.bmp> -s <secret.txt> -o <output.bmp>\n\n");
		printf("\t\t*** DECODING *** ==> \t./a.out -D -i <output.bmp> -o <output_text.txt>\n\n");
		printf("\t\tFOR MORE DETAILS ===> **README**\n");

    }	
    return 0;
}

