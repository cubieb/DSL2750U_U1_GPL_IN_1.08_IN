#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "id3tag.h"
#include "genre.dat"


static char* ucs4_extract_iso_8859_1(const id3_ucs4_t* a)
{
	char* b;
	int i, len;

    if (a == NULL)
    {
        return a;
    }
	
	len = 0;
	
	while(a[len]) len++;
	
	b = (char*)malloc(sizeof(char)*(len+1));
	assert(b);
	
	for(i=0; i<len; i++) {
		b[i] = (a[i] > 255) ? '_' : a[i];
	}

	b[len] = '\0';
	
	return b;
}

static char* ucs4_extract_utf8(const id3_ucs4_t* a) {
	char* b;
	int i, len;

	len = 0;

	while(a[len]) len++;

	b = (char*)malloc(sizeof(char)*(len+1));
	assert(b);

	for(i=0; i<len; i++) { 
		b[i] = (a[i] > 255) ? '_' : a[i];
	}

	b[len] = '\0';
	
	return b;
}

static char* tag_get_textframe(struct id3_tag* id3tag, const char* framename)
{
	struct id3_frame* id3frame = NULL;
	union id3_field* id3field = NULL;
	char* s;
	int encoding, id3fieldtype;
	
	s = (char*)NULL;
	id3frame = id3_tag_findframe(id3tag, framename, 0);

	if(id3frame) {
		/* get encoding */
		id3field = id3_frame_field(id3frame, 0);
		id3fieldtype = id3_field_type(id3field);
		assert(id3fieldtype == ID3_FIELD_TYPE_TEXTENCODING);
		encoding = id3_field_gettextencoding(id3field);
		switch(encoding) {
			case ID3_FIELD_TEXTENCODING_ISO_8859_1:
				id3field = id3_frame_field(id3frame, 1);
	 			assert(id3field);
				s = ucs4_extract_iso_8859_1(id3_field_getstrings(id3field, 0));
				id3_frame_delete(id3frame);
				break;
			case ID3_FIELD_TEXTENCODING_UTF_8:
			{
				id3field = id3_frame_field(id3frame, 1);
				assert(id3field);
				s = ucs4_extract_utf8(id3_field_getstrings(id3field, 0));
				id3_frame_delete(id3frame);
				break;
			}
		}
	}

	return s;
}

struct id3_file *id3file = NULL;
struct id3_tag *id3tag = NULL;

int InitAudioTagInfo(const char *filename)
{
	id3file = id3_file_open(filename, ID3_FILE_MODE_READONLY);
	if(!id3file) {
		printf("could not open file '%s'\n", filename);
		return -1;
	}
	
	id3tag = id3_file_tag(id3file);
	if(!id3tag) {
		printf("could not get id3-tag for file '%s'\n", filename);
		id3_file_close(id3file);
		return -1;
	}

	return 0;
}

char *GetAudioTagInfo(const char *tag_name)
{
	char *temp = NULL;
	if (!strcmp(tag_name, "TCON"))
	{
		temp = tag_get_textframe(id3tag, tag_name);
		if (temp)
		{
		return ucs4_extract_iso_8859_1(genre_table[atoi(tag_get_textframe(id3tag, tag_name))]);
		}
		else
			return NULL;
	}
	else
		return tag_get_textframe(id3tag, tag_name);
}

void FinishAudioTagInfo(void)
{ 
	id3_tag_delete(id3tag);
	id3_file_close(id3file);
}
