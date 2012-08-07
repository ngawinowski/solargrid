#include <stdio.h>

main()	{
	char s[32];
	double d = 2.5;
	sprintf(s, "%.3f", d);
	printf("%s\n",s);
	printf("s = %s \n", s);
	unsigned char us[sizeof(s)];
	int sl = sizeof(s);
	int i;
	for (i=0; i <= sl; i++) {
		us [i] = s [i];
	}
		
	printf("%s\n",us);
}

