#ifdef _AUDIO_GET_TAG_INFO_H
#undef _AUDIO_GET_TAG_INFO_H

typedef unsigned char id3_byte_t;

int InitAudioTagInfo(const char *filename);
char *GetAudioTagInfo(const char *tag_name);
void FinishAudioTagInfo(void);

#endif
