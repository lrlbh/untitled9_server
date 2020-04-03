#include "hash_lr.h"

hash_lr::hash_lr(hash_type type)
{
	this->init(type);

}

hash_lr::~hash_lr()
{

}

void hash_lr::init(hash_type type)
{
	this->type = type;
	switch (type)
	{
	case type_md5:
		MD5_Init(&this->md5_ctx);
		break;

	case type_sha1:
		SHA1_Init(&this->sha1_ctxl);
		break;

	case type_sha224:
		SHA224_Init(&this->sha224_ctx);
		break;

	case type_sha256:
		SHA256_Init(&this->sha256_ctx);
		break;

	case type_sha384:
		SHA384_Init(&this->sha384_ctx);
		break;

	case type_sha512:
		SHA512_Init(&this->sha512_ctx);
		break;

	default:
		MD5_Init(&this->md5_ctx);
		break;
	}
}

void hash_lr::add_data(char* data)
{
	switch (this->type)
	{
	case type_md5:
		MD5_Update(&this->md5_ctx,data,strlen(data));
		break;

	case type_sha1:
		SHA1_Update(&this->sha1_ctxl, data, strlen(data));
		break;

	case type_sha224:
		SHA224_Update(&this->sha224_ctx, data, strlen(data));
		break;

	case type_sha256:
		SHA256_Update(&this->sha256_ctx, data, strlen(data));
		break;

	case type_sha384:
		SHA384_Update(&this->sha384_ctx, data, strlen(data));
		break;

	case type_sha512:
		SHA512_Update(&this->sha512_ctx, data, strlen(data));
		break;

	default:
		MD5_Update(&this->md5_ctx, data, strlen(data));
		break;
	}
}

string hash_lr::result()
{

	switch (this->type)
	{
	case type_md5:
		return this->md5_result();
		break;

	case type_sha1:
		return this->sh1_result();
		break;

	case type_sha224:
		return this->sha224_result();
		break;

	case type_sha256:
		return this->sha256_result();
		break;

	case type_sha384:
		return this->sha384_result();
		break;

	case type_sha512:
		return this->sha512_result();
		break;

	default:
		return this->md5_result();
		break;
	}


	
}

string hash_lr::md5_result()
{
	unsigned char md[MD5_DIGEST_LENGTH];
	char res[MD5_DIGEST_LENGTH * 2 + 1];
	MD5_Final(md,&this->md5_ctx);
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(&res[i*2],"%02x",md[i]);
	return string(res,MD5_DIGEST_LENGTH*2+1);
}

string hash_lr::sh1_result()
{
	unsigned char md[SHA_DIGEST_LENGTH];
	char res[SHA_DIGEST_LENGTH * 2 + 1];
	SHA1_Final(md, &this->sha1_ctxl);
	for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA_DIGEST_LENGTH * 2 + 1);
}

string hash_lr::sha224_result()
{
	
	unsigned char md[SHA224_DIGEST_LENGTH];
	char res[SHA224_DIGEST_LENGTH * 2 + 1];
	SHA224_Final(md, &this->sha224_ctx);
	for (int i = 0; i < SHA224_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA224_DIGEST_LENGTH * 2 + 1);
}

string hash_lr::sha256_result()
{
	unsigned char md[SHA256_DIGEST_LENGTH];
	char res[SHA256_DIGEST_LENGTH * 2 + 1];
	SHA256_Final(md, &this->sha256_ctx);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA256_DIGEST_LENGTH * 2 + 1);
}

string hash_lr::sha384_result()
{
	unsigned char md[SHA384_DIGEST_LENGTH];
	char res[SHA384_DIGEST_LENGTH * 2 + 1];
	SHA384_Final(md, &this->sha384_ctx);
	for (int i = 0; i < SHA384_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA384_DIGEST_LENGTH * 2 + 1);
}

string hash_lr::sha512_result()
{
	unsigned char md[SHA512_DIGEST_LENGTH];
	char res[SHA512_DIGEST_LENGTH * 2 + 1];
	SHA512_Final(md, &this->sha512_ctx);
	for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA512_DIGEST_LENGTH * 2 + 1);
}
