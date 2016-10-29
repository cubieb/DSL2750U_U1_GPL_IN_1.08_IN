#include "CdsObject.h"
#include "CdsDidlSerializer.h"
#include "libavformat/avformat.h"
extern void _CDS_Clone_MultipleStrings(char ***strings, int num_strings, const char** clone_this);

void InitFFMpeg()
{
    av_register_all();
}
int FillResInfo(struct CdsObject *cdsObj,struct CdsResource *res)
{
    AVFormatContext *ctx;
    char *filename;
    int result;
    int i;
    AVMetadataTag   *album, *genre, *track;
    if (!(cdsObj->MediaClass & CDS_CLASS_MASK_ITEM))
    {
        return -1;
    }
//    printf("cdsObj is %p \n",cdsObj);
//    printf("cdsObj->Source is %p \n",cdsObj->Source);
    filename=cdsObj->Source;
    if ((result=av_open_input_file (&ctx, filename, NULL, 0, NULL)) != 0)
    {
        fprintf (stderr, "can't open file: %s %d\n", filename,result);
        return -1;
    }
    if (av_find_stream_info (ctx) < 0)
    {
        fprintf (stderr, "can't find stream info\n");
        return -1;
    }
    /*
       dump_format (ctx, 0, NULL, 0);
     */
    if (cdsObj->MediaClass & CDS_CLASS_MASK_MAJOR_AUDIOITEM)
    {

        if (ctx->metadata)
        {
            genre=av_metadata_get(ctx->metadata,"genre",NULL,0);
            album=av_metadata_get(ctx->metadata,"album",NULL,0);
            track=av_metadata_get(ctx->metadata,"track",NULL,0);
            if (genre)
            {
//                cdsObj->TypeMajor.AudioItem.NumGenres=1;
//                _CDS_Clone_MultipleStrings(&cdsObj->TypeMajor.AudioItem.Genres,1,&genre->value);
                printf("zhc:the %s genre is %s\n",filename,genre->value);
            }
            if (album)
            {
//                cdsObj->TypeMajor.AudioItem.NumAlbums=1;
//                _CDS_Clone_MultipleStrings(&cdsObj->TypeMajor.AudioItem.Albums,1,&album->value);
                printf("zhc:the %s album is %s\n",filename,album->value);
            }
        }
    }
    av_close_input_file (ctx);
    return 0;
}
