#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define	ERRFILEREAD	-1
#define	ERRFILEWRITE	-2
#define	ERRFILESEEK	-3

int addlinenum(int ifd, int ofd);

int main(int argc, char **argv)
{
	int ifd, iret;	// Variables for input file
	int ofd, oret;	// Variables for output file
	int ret;
	char err1[64] = "Need More Argument!\n";
	char err2[64] = "Too Many Arguments!\n";
	char err3[64] = "Input file is not exist!\n";
	char err4[64] = "Output file has problem!\n";
	char err5[64] = "Error while inserting number\n";
	char err6[64] = "Error while closing input file\n";
	char err7[64] = "Error while closing output file\n";

	if (argc < 3) {		// When there are not enough variables
		write(STDOUT_FILENO, err1, 20);
		exit(1);
	} else if (argc > 3) {	// When there are so many variables
		write(STDOUT_FILENO, err2, 20);
		exit(1);
	} else {
		/* Open input file */
		ifd = open(argv[1], O_RDONLY);
		if (ifd < 0) {
			write(STDOUT_FILENO, err3, 25);
			exit(1);
		}

		/* Create and open output file */
		ofd = open(argv[2], O_RDWR | O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if (ofd < 0) {
			write(STDOUT_FILENO, err4, 25);
			exit(1);
		}
		
		/* Add line number to output file */
		ret = addlinenum(ifd, ofd);
		if (ret < 0) {
			write(STDOUT_FILENO, err5, 29);
			exit(1);
		}

		/* Close input file */
		iret = close(ifd);
		if (iret < 0) {
			write(STDOUT_FILENO, err6, 31);
			exit(1);
		}

		/* Close output file */
		oret = close(ofd);
		if (oret < 0) {
			write(STDOUT_FILENO, err7, 32);
			exit(1);
		}
	}

	exit(0);
}

#define	RADIX	10

int IntToChar(int value, char *buf)
{
	int _value = value;
	int index = 0;
	int i;
	char temp;

	while (_value) {
		buf[index] = (_value % RADIX) + '0';
		_value /= RADIX;
		index++;
	}

	for (i = 0; i < index / 2; i++) {
		temp = buf[i];
		buf[i] = buf[(index - 1) - i];
		buf[(index - 1) -i] = temp;
	}

	return 0;
}

int addlinenum(int ifd, int ofd)
{
	return 0;
}

