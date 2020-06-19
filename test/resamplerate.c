/*************************************************************************
	> File Name: resamplerate.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Fri Aug  5 16:25:02 2016
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>

#include "samplerate.h"
// #include "iniparser.h"
// #include "log.h"
#include "resamplerate.h"
// #include "handle_wave.h"


typedef struct conf
{
    int inputSampleRate;
    int outputSampleRate;
    int channels;
    int inFrameSize;
    int outFrameSize;
}ResampConf;

AUDIO* initialize(ResampConf config)
{
    AUDIO *retVal = (AUDIO *)calloc(1,sizeof(AUDIO));
    retVal->input_sample_rate = config.inputSampleRate;
    retVal->output_sample_rate = config.outputSampleRate;
    retVal->channels = config.channels;
    retVal->inFrameSize = config.inFrameSize;
    retVal->ratio = (retVal->output_sample_rate / retVal->input_sample_rate);

    retVal->outFrameSize = (int)(retVal->ratio + 1) * retVal->inFrameSize;

    return retVal;
}

int main(int argc,const char *argv[])
{
    int  error;
    int blockSize;
    int frameSize;
    int blockSize2;
    AUDIO *ResampEng;
    SRC_DATA samplerate;
    SRC_STATE *state;
    FILE *in_fp;
    FILE *out_fp;
    float *inputBuf;
    float *outputBuf;
    short *tmpBuf;
    size_t nread = 0;

    float duration;
    struct timeval tv1;
    struct timeval tv2;
    struct timezone tz;
    if(argc < 3)
    {
        printf("*******************************************************"
           "******************\n"
           " Copyright (c) 2015 - 2025 AISPEECH.\n"
           " usage   :\n"
           " %s <input pcm> <output pcm> \n"
           "*******************************************************"
           "******************\n", argv[0]);
        return -1;
    }

    ResampConf config;
    memset(&config,0,sizeof(config));
    config.inputSampleRate = 48000;
    config.outputSampleRate = 16000;
    config.channels = 1;
    config.inFrameSize = 1536;

    ResampEng = initialize(config);

    blockSize = sizeof(short) * ResampEng->channels;
    blockSize2 = sizeof(float) * ResampEng->channels;
    frameSize = ResampEng->inFrameSize;

    inputBuf = (float*)calloc(1 , blockSize2 * frameSize);
    if(inputBuf == NULL)
    {
        printf("inputBuf calloc failed\n");
        return -1;
    }
    outputBuf = (float*)calloc(1, blockSize2 * ResampEng->outFrameSize);
    if(outputBuf == NULL)
    {
        printf("outputBuf calloc failed\n");
        return -1;
    }
    tmpBuf = (short*)calloc(1, blockSize * ResampEng->outFrameSize);
    if(tmpBuf == NULL)
    {
        printf("outputBuf calloc failed\n");
        return -1;
    }

    in_fp = fopen(argv[1],"rb");
    if (in_fp == NULL)
    {
        printf("open input file failed");
        return -1;
    }
	//fseek(in_fp,44,SEEK_SET);
    out_fp = fopen(argv[2], "wb+");
    if (out_fp == NULL)
    {
        printf("create output file failed");
        return -1;
    }

    /* samplerate params config */
    samplerate.data_in = inputBuf;
    samplerate.input_frames = ResampEng->inFrameSize;
    samplerate.data_out = outputBuf;
    samplerate.end_of_input = 0;
    samplerate.src_ratio = ResampEng->ratio;
    samplerate.output_frames = ResampEng->outFrameSize;

    if ((state = src_new (SRC_LINEAR, ResampEng->channels, &error)) == NULL)
    {
        printf("resample new failed\n");
        return -1;
        // exit(EXIT_FAILURE);
    }

    gettimeofday(&tv1,&tz);
    /* resamplerate */
    while ((nread = fread(tmpBuf,blockSize,frameSize,in_fp)) > 0)
    {
        // src_short_to_float_array (tmpBuf, inputBuf, nread*ResampEng->channels) ;
        src_short_to_float_array (tmpBuf, inputBuf, nread) ;

        if ((error = src_process (state, &samplerate)))
        {
            printf("src_process failed : %s\n",src_strerror (error)) ;
        }

        memset(tmpBuf,'\0',ResampEng->outFrameSize * blockSize);
        src_float_to_short_array (outputBuf, tmpBuf, samplerate.output_frames_gen * ResampEng->channels);

        if (fwrite(tmpBuf,blockSize,samplerate.output_frames_gen,out_fp) != samplerate.output_frames_gen)
        {
            printf("fwrite failed\n");
        }
        // printf("%d\n", samplerate.output_frames_gen);
    }
    gettimeofday(&tv2,&tz);
    
    duration = (float)(tv2.tv_sec - tv1.tv_sec) + (float)(tv2.tv_usec-tv1.tv_usec)/1000000;
    printf("duration:%f s\n", duration);

    // printf("output path : %s\n",output.path);
    printf("successfully\n");

    if(tmpBuf)
    {
        free(tmpBuf);
        tmpBuf = NULL;
    }
    if(inputBuf)
    {
        free(inputBuf);
        inputBuf = NULL;
    }
    if(outputBuf)
    {
        free(outputBuf);
        outputBuf = NULL;
    }
    if(ResampEng)
    {
        free(ResampEng);
    }
    if(in_fp)
    {
        fclose(in_fp);
    }

    if(out_fp)
    {
        fclose(out_fp);
    }

    // clean_up();
    src_delete(state);

    return 0;
}
