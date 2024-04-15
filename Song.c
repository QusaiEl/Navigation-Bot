/*#include "Song.h"*/
#include "Song.h"



void bear(oi_t *sensor_data)
{
    unsigned char song3[]= {60,62,65,69,68,69,67,65,65,67,65,67,65,67,65,62};
    unsigned char duration3[] = {20,20,20,40,15,20,20,20,20,20,20,20,20,20,20,30};
//    int music= sensor_data ->songPlaying;
    oi_update(sensor_data);
//    lcd_printf("Uh Oh! The Shelves\nAre bare of\nnecessities\nBattery: %d", battery);
    oi_loadSong(3, 16, song3, duration3);
    timer_waitMillis(1000);
    oi_play_song(3);
//oi_free(sensor_data);
}



//void wonder(oi_t *sensor_data)
//{
//    unsigned char song2[]= { 81,78,74,0, 81,78,74,0, 74, 83, 81,79};
//    unsigned char duration[] = {22,13,13,1,22,13,13,3,36,36,36,36};
////69,66,62
//
//
////    int music= sensor_data ->songPlaying;
//    oi_update(sensor_data);
//    lcd_printf("Wonder Pets!\nWonder Pets!\nWe're on our way!\nBattery: %d", battery);
//    oi_loadSong(2, 12, song2, duration);
//    timer_waitMillis(1000);
//    oi_play_song(2);
//
////oi_free(sensor_data);
//}
//
//
//
//
//void gummy(oi_t *sensor_data)
//{
//
//    unsigned char song1[]= {64,64, 64, 62,62,62,62,62,62,62,64,62,60,57};
//    unsigned char duration[] = {14,14,14,14,14,14,14,14,14,14,14,14,14,28};
//
//
//
////    int music= sensor_data ->songPlaying;
//    oi_update(sensor_data);
//    lcd_printf("Who am I you ask?\nI'm a GUMMMMY BEARRR\nBattery: %d", battery);
//    oi_loadSong(1, 16, song1, duration);
//    timer_waitMillis(1000);
//    oi_play_song(1);
//
////oi_free(sensor_data);
//
//}
//void (oi_t *sensor_data)
//{
//
//    unsigned char song4[]= {64,64, 64, 62,62,62,62,62,62,62,64,62,60,57};
//    unsigned char duration4[] = {14,14,14,14,14,14,14,14,14,14,14,14,14,28};
//
//
//
////    int music= sensor_data ->songPlaying;
//    oi_update(sensor_data);
//    lcd_printf("Who am I you ask?\nI'm a GUMMMMY BEARRR\n \nBattery: %d", battery);
//    oi_loadSong(1, 16, song4, duration4);
//    timer_waitMillis(1000);
//    oi_play_song(1);
//
////oi_free(sensor_data);
//
//}
//
//


/*
 * void oi_loadSong(int song_index, int num_notes, unsigned char  *notes, unsigned char  *duration);
brief Play song
param An integer value from 0 - 15 that is a previously establish song index
void oi_play_song(int index);
 *

 *  //Music
    uint8_t songNumber;
    uint8_t songPlaying;
 *
 */
