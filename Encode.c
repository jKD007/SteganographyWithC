
int size_of_image(FILE *fp1)
{
	int width, height;

	fseek(fp1, 0x12, SEEK_SET);	// dua con tro file tu dau-offset la 0x12
	fread(&width, sizeof(int), 1, fp1);	// doc weight 4 byte ke tiep
	fread(&height, sizeof(int), 1, fp1);	// doc height 4 byte ke tiep
	printf("Dimensions of the Image is %d x %d \n", width, height);
	fseek(fp1, 0L, SEEK_SET);	// dua con file ve vi tri ban dau

	return ((width * height * 3) / 8); // tra ve gia tri rong*cao*/8
}
//Ghi secret text vao file bang cach ghi tung ki tu nhap tu ban phim cho den khi gap EOF thi ngung 
void secret_text(FILE *fp2)
{
	char ch;
	while ((ch = getchar())!=EOF)
	{
		putc(ch,fp2);	//dua text vao con tro file
	}
}

// tra ve size cua file secret
int secret_text_size(FILE *fp2)
{
	int size_txt;
	fseek(fp2, 0L, SEEK_END);// dua con tro den cuoi file 
	size_txt = ftell(fp2);// dung ham ftell() để lấy tổng kích thước của một file sau khi den cuoi file
	fseek(fp2, 0L, SEEK_SET);// dua con tro ve dau file
	return size_txt;// tra ve kich thuoc cua file
}
// Lay gia tri bit trong 1 byte theo vi tri chi dinh
int get_bit(char byte, int bit)
{
	return ((byte >> 8 - bit) & 1);
}

// Thuat toan lsb cho file secret
void stega_encrypt(FILE *fp1, FILE *fp2, FILE *fp3)
{
	char file_buff = 0, msg_buff = 0, ch;	
	int i;
	int bit_msg;
	while((msg_buff = fgetc(fp2)) != EOF)// Lay byte tu file secret 
	{
		for(i = 1; i <= 8; i++)
		{
			file_buff = fgetc(fp1);//Lay byte tu file input

			int file_byte_lsb = (file_buff & 1);// bit thap cua byte lay tu input

			bit_msg = get_bit(msg_buff, i);// lay bit thu i trong 1 byte cua msg_buff

			if(file_byte_lsb == bit_msg) // tuong tu
			{
				fputc(file_buff, fp3);
			}
			else
			{
				if(file_byte_lsb == 0)
					file_buff = (file_buff | 1);
				else
					file_buff = (file_buff ^ 1);

				fputc(file_buff, fp3);
			}
		}
	}

	//Sau khi đã duyệt hết file secret, ghi phần còn lại của file bmp input vào file bmp output
	while(!feof(fp1))
	{
		char tmp_cpy = fgetc(fp1);
		fputc(tmp_cpy,fp3);

	}

	if(msg_buff == EOF)
		printf("\n*** Secret Message Encrypted Successfully ***\n");
	else
		printf("\n*** Failed Encrypting ***\n");
}
//magic_str,fp1,fp3
void string_encrypt(char *str,FILE *fp1, FILE *fp3)
{

	char file_buff, msg_buff;	
	int i, j = 0;
	int bit_msg;
	while((msg_buff = str[j]) != '\0')// doc tung byte cua magic string
	{
		for(i = 1; i <= 8; i++)
		{
			file_buff = fgetc(fp1);// doc tung byte cua fp1 input.bmp

			int file_byte_lsb = (file_buff & 1);// lay bit thap nhat cua byte lay tu input.bmp

			bit_msg = get_bit(msg_buff, i);//sau do lay gia tri bit thu i trong 1 byte cua magic_buff

			if(file_byte_lsb == bit_msg)// neu bit thap nhat cua byte lay tu fp1 = bit_msg thi ghi vao file out.bmp
			{
				fputc(file_buff, fp3);
			}
			else// tuong tu
			{
				if(file_byte_lsb == 0)
					file_buff = (file_buff | 1);
				else
					file_buff = (file_buff ^ 1);

				fputc(file_buff, fp3);
			}
		}
		j++;
	}
}
//num la size cua magic string fp1=input.bmp, fp3=output.bmp
void size_encrypt(int num,FILE *fp1, FILE *fp3)
{

	char file_buff;	
	int i, j = 0;
	int bit_msg;

	for(i = 1; i <= 8; i++)
	{
		file_buff = fgetc(fp1);// lay tung byte cua input.bmp

		int file_byte_lsb = (file_buff & 1); // lay gia tri cua bit thap nhat

		bit_msg = get_bit(num, i); // sau do lay gia tri bit thu i trong 1 byte cua num

		if(file_byte_lsb == bit_msg)// neu bit_lsb=bit_msg thi dua byte lay tu file input vao output
		{
			fputc(file_buff, fp3);
		}
		else // nguoc lai 
		{	// neu bit thap nhat cua ki tu lay tu fp1=0 thi or 1 cho giong bit_msg
			if(file_byte_lsb == 0)
				file_buff = (file_buff | 1);
			else // neu bit thap nhat cua ki tu lay tu fp1=1 thi xor 1 cho giong bit_msg
				file_buff = (file_buff ^ 1); 

			fputc(file_buff, fp3);// sau do dua byte do vao output
		}
	}

}

// 3 tham so tu -i -s -o
int Encoding(char *argv_2,char *argv_3,char *argv_4)
{
	FILE *fp1, *fp2, *fp3;

	// mo file Input Source BMP Image de giau anh neu khong
	if((fp1 = fopen(argv_2, "rb+")) == NULL) // r+ Mo tep o che do doc va ghi, neu file ko co thi thoat
	{
		printf("Could not open file %s.\nAborting\n", argv_2);
		return 1;
	}
	// so luong byte co the luu trong anh
	int size_image = size_of_image(fp1);
	printf("Total %d Charecters can be stored in %s.\n", size_image, argv_2);

	// tao file secret.txt
	fp2 = fopen(argv_3, "wb+");

	//Nhap vao noi dung can luu se luu vao file secret.txt
	printf("Enter your secret text and Press CTRL + D To Stop : \t");
	secret_text(fp2);
	// lay size file secret.txt
	int size_txt = secret_text_size(fp2);
	printf("\nSize of The Secret Message is ==> %d\n", size_txt);

	// So sanh size file cua anh bmp vs size cua file secret.txt neu size bmp< size secret thi gioi han cua file bmp ko du de giau anh
	if(size_image < size_txt)
	{
		printf("\n*** The Size Of Message Exceeds the Size of image ***\n");
		return 1;
	}

	//tao file out.bmp
	fp3 = fopen(argv_4, "wb+");
	if (fp3== NULL) 
	{
		fprintf(stderr, "Cannot create output file %s\n",argv_4);
		exit(1);
	}

	//copy 54 byte cua file input.bmp vao file output.bmp
	int i, c = 0;
	char tmp_cpy;
	rewind(fp1);	//dua con tro file tới phần đầu của file
	for(i = 0;i < 54; i++)// 54 byte
	{
		tmp_cpy = fgetc(fp1);// doc 1 byte cua input.bmp
		fputc(tmp_cpy,fp3);// ghi 1 byte da doc vao out.bmp
		c++;// 
	}

	//checking 54 bytes copyed or not
	if(i == c)
		printf("\n*** Sucessfully copied 54 header bytes ***\n");
	else
	{
		printf("\n*** Failed to copy 54 header bytes ***\n");
		return 0;
	}

	//For Magic String
	
	char magic_str[10];
	char magic;
	int size;

	printf("\nEnter The Magic String '#' '$' Only : \t");

	for(i = 0; (magic = getchar()) != '\n'; i++)
	{
		if(magic == '#')
		{
			magic_str[i] = magic;
		}
		else if(magic == '$')
		{
			magic_str[i] = magic;
		}
		else
		{
			printf("*** You Entered Wrong Charecters ***\n");
			exit(2);
		}
	}

	magic_str[i] = '\0';
	size = strlen(magic_str);// Lay size cua magic string
	printf("Size Of The Magic string is ==> %d\n", size);
	
	// dua byte cua fp1 da xu li vao fp3 theo size magic string
	size_encrypt(size,fp1,fp3);
	//dua byte cua fp1 da xu li vao fp3 theo magic string
	string_encrypt(magic_str,fp1,fp3);
	printf("*** Magic String Encrypted ***\n");

	//FOR PASSWORD
	char str[20];
	printf("\nEnter the password : \t");
	// Nhap pass vao str
	for (i=0;((str[i] = getchar()) != '\n');i++);
	str[i]= '\0';

	int size1;	
	size1 = strlen(str);
	printf("Size Of The Password is ==> %d\n", size1);
	//dua byte cua fp1 da xu li vao fp3 theo size pass
	size_encrypt(size1,fp1,fp3);
	//dua byte cua fp1 da xu li vao fp3 theo pass
	string_encrypt(str,fp1,fp3);
	printf("*** Password Encrypted ***\n");

	//dua byte cua fp1 da xu li vao fp3 theo size file secret
	size_encrypt(size_txt,fp1,fp3);

	// dua byte cua secret.txt vao file output.
	stega_encrypt(fp1,fp2,fp3);

	/*Clean up before exit */
	fclose(fp1);	
	fclose(fp2);
	fclose(fp3);

	return 0;

}
