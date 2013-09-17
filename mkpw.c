#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>

#ifdef __APPLE_CC__
#include <CommonCrypto/CommonDigest.h>
#else
#include <openssl/sha.h>
#endif

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s service\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *service = argv[1];
	char *p = "Password for ";
	char prompt[strlen(service) + strlen(p) + 3];
	sprintf(prompt, "%s%s: ", p, service);

	char *pw = getpass(prompt);
	char buf[strlen(pw) + strlen(service) + 1];
	sprintf(buf, "%s%s", service, pw);

#ifdef __APPLE_CC__
	unsigned char md_value[CC_SHA1_DIGEST_LENGTH];
	int md_len = CC_SHA1_DIGEST_LENGTH;

	CC_SHA1_CTX c;
	CC_SHA1_Init(&c);
	CC_SHA1_Update(&c, buf, strlen(buf));
	CC_SHA1_Final(md_value, &c);
#else
	unsigned char *md_value = SHA1((unsigned char *) buf, strlen(buf), NULL);
	int md_len = SHA_DIGEST_LENGTH;
#endif

	int i;
	for (i = 0; i < md_len; i++) {
		printf("%02x ", md_value[i]);
	}

	printf("\n");
}
