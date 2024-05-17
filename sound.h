#ifndef SOUND_H
#define SOUND_H

#define IOCTL_START_BUZZER      _IOW('b', 0x07, int)
#define IOCTL_END_BUZZER        _IOW('b', 0x09, int)
#define IOCTL_SET_TONE          _IOW('b', 0x0b, int)
#define IOCTL_SET_VOLUME        _IOW('b', 0x0c, int)

extern int buzzer_fd;
extern const int BGM_VOLUME;

typedef struct {
    double frequency;
    int duration_us;
} Note;

void startBuzzer();
void stopBuzzer();
void changeTone(long tone, int volume);
void playSound(double frequency, int volume, int duration_us);
void playGameStartSound();
void playGameOverSound();
void playVictorySound();
void playPelletSound();
void playPowerupSound();
void playGhostCollisionSound();
void playBackgroundMusic();

#endif