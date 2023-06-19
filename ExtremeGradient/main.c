#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <malloc.h>

void main()
{
	unsigned int* a = malloc(1073741824);//1gb
	if (a == NULL) return;

	for (unsigned int i = 0; i < 268435456; i++)
		a[i] = i << 4;

	FILE* r;
	r = fopen("raw.tmp", "wb");
	if (r == NULL) return;

	unsigned short z[16384] = { 0 };
	//frame döngüsü
	for (int f = 0; f <32768; f++)
	{
		long long b[16384] = { 0 };

		for (long long i = 0; i < 268435456; i++)
			b[i / 16384] += a[i];

		unsigned short c[16384];
		for (short j = 0; j < 16384; j++)
			c[j] = b[j] >> 30;

		fwrite(z, 2, 16384, r);
		fwrite(c, 2, 16384, r);
		fwrite(c, 2, 16384, r);
		fwrite(c, 2, 16384, r);
		fwrite(z, 2, 16384, r);

		printf("%d\n", f);

		for (int i = 0; i < 268435456; i++)
			a[i] += f*f;
	}
	fclose(r);
}
//ffmpeg -y -f rawvideo -pix_fmt gray16le -s:v 16384x5 -r 60 -i raw.tmp -sws_flags gauss -vf scale=1920:1080 -preset ultrafast -crf 1 ultrafast-fastdecode.mp4
