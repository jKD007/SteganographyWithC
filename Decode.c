// giai ma size
void size_decryption(FILE *pf1, int *size_txt)
{
	int file_buff = 0, i;
	int ch, bit_msg;
	for (i = 0; i < 8; i++)
	{
		ch = fgetc(pf1);// lay 4 byte trong input.bmp 
		bit_msg = (ch & 1);// 4 byte and vs 1 ra byte thap nhat
		if (bit_msg) // neu bit_msg=1 thi
		{
			file_buff = (file_buff << 1) | 1;
		}
		else
		{
			file_buff = file_buff << 1;
		}
	}// Duyet 8 byte dau tuong ung vs 8 byte int num
	// de lay trong moi byte bit thap nhat roi ghep lai thang int num
	*size_txt = file_buff;
}
// giai ma magic string
// do dai dc giau trc magic string
void string_decryption(FILE *pf1,char *strng,int size)
{
	int file_buff=0, i, j=0, k=0;
	int ch, bit_msg;
	for (i = 0; i < (size * 8); i++)// moi byte cua string duoc giau trong 8 byte cua file input.bmp
	{
		j++;
		ch = fgetc(pf1);
		bit_msg = (ch & 1);// lay bit thap nhat cua file anh ghep vao ben phai byte string
		if (bit_msg)
		{
			file_buff = (file_buff << 1) | 1;
		}
		else
		{
			file_buff = file_buff << 1;
		}

		if ( j == 8)// den bit thu 8, ep byte thanh ki tu roi ghep ben phai string
		{
			strng[k] =(char)file_buff; 
			j=0;
			k++;
			file_buff = 0;
		}
	}
	strng[k] = '\0';
}
// giai ma file output.txt
void secret_decryption(int size_txt, FILE *pf1, FILE *pf2)
{
	int file_buff=0, i, j = 0, k = 0;
	int ch,bit_msg;
	char output[250] = {0};
	for (i = 0; i < (size_txt * 8); i++)// moi byte cua secret.txt duoc giau trong 8 byte cua file anh
	{
		j++;
		ch = fgetc(pf1);
		bit_msg = (ch & 1);
		if (bit_msg)// lay bit thap nhat file anh ghep vao ben phai byte output
		{
			file_buff = (file_buff << 1) | 1;
		}
		else
		{
			file_buff = file_buff << 1;
		}

		if ( j == 8)// cho den bit thu 8 ep kieu thanh ki tu roi ghi ra out.txt
		{
			putc(file_buff, pf2);
			output[k++] = file_buff;
			j=0;
			file_buff = 0;
		}
	}
	printf("\n*** Secret Text Is ==> %s\n\n", output);
}


// Giai ma
int Decode(char *argv_2, char *argv_4)
{
	FILE *pf1, *pf2;
	
	int size, size1, size_txt;

	//Kiem tra file decode co mo dc ko
	if((pf1 = fopen(argv_2, "rb")) == NULL) 
	{
		printf("Could not open file %s.\nAborting\n", argv_2);
		return 1;
	}
	// dua con tro file den vi tri 54
	fseek(pf1,54 ,SEEK_SET);
	// tao file secret2.txt
	if((pf2 = fopen(argv_4, "wb+")) == NULL) 
	{
		printf("Could not open file %s.\nAborting\n", argv_4);
		return 1;
	}

	//kiem tra magic string

	char magic_strn[20], magic_strn1[20], strng[20];
	int j;

	size_decryption(pf1,&size);
	string_decryption(pf1,magic_strn,size);

	printf("\nEnter the  magic string : \t");
	for(j = 0;(magic_strn1[j] = getchar()) != '\n'; j++);
	magic_strn1[j]= '\0';

	int temp;
	temp = strcmp(magic_strn1, magic_strn);

	if(temp)
	{
		printf("\n\t*** Entered Wrong Magic String ***\n");
		return 0;
	}
	else
		printf("\n\t*** Magic String Accepted ***\n\n");

	//kiem trapassword
	char passwd[20], passwd_str[20];
	
	size_decryption(pf1,&size1);
	string_decryption(pf1,passwd_str,size1);

	printf("Enter The Password : \t");
	scanf("%s", passwd);

	temp = strcmp(passwd,passwd_str);

	if(temp)
	{
		printf("\n\t*** Entered Wrong Password ***\n");
		return 0;
	}
	else
		printf("\n\t*** Password Accepted ***\n");

	// giai ma ra file out.txt
	size_decryption(pf1, &size_txt);
	secret_decryption(size_txt, pf1, pf2);

	printf("*** The Secret Text Is Copied To ==> %s\n\n", argv_4);

	/* closeing files */
	fclose(pf1);

}
