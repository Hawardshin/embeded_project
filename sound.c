#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "sound.h"

const int BGM_VOLUME = 10000;
int buzzer_fd;

Note bgm[] = {
    {440.00, 200000}, // A4
    {523.25, 200000}, // C5
    {659.25, 200000}, // E5
    {783.99, 200000}, // G5
    {659.25, 200000}, // E5
    {523.25, 200000}, // C5
    {440.00, 200000}, // A4
    {0, 100000}, // 휴식
};

int bgm_index = 0;
int bgm_size = sizeof(bgm) / sizeof(Note);

long freqToTone(double freq) {
    return (long)((1.0 / freq) * 1000000000);
}

void startBuzzer() {
    if (ioctl(buzzer_fd, IOCTL_START_BUZZER, 0) == -1) {
        perror("ioctl IOCTL_START_BUZZER");
        exit(EXIT_FAILURE);
    }
}

void stopBuzzer() {
    if (ioctl(buzzer_fd, IOCTL_END_BUZZER, 0) == -1) {
        perror("ioctl IOCTL_END_BUZZER");
        exit(EXIT_FAILURE);
    }
}

void changeTone(long tone, int volume) {
    if (ioctl(buzzer_fd, IOCTL_SET_VOLUME, volume) == -1) {
        perror("ioctl IOCTL_SET_VOLUME");
        exit(EXIT_FAILURE);
    }
    if (ioctl(buzzer_fd, IOCTL_SET_TONE, tone) == -1) {
        perror("ioctl IOCTL_SET_TONE");
        exit(EXIT_FAILURE);
    }
}

void playSound(double frequency, int volume, int duration_us) {
    long tone = freqToTone(frequency);
    changeTone(tone, volume);
    startBuzzer();
    usleep(duration_us);
    stopBuzzer();
}

void playGameStartSound() {
    playSound(440.00, BGM_VOLUME, 200000); // A4
    playSound(523.25, BGM_VOLUME, 200000); // C5
    playSound(659.25, BGM_VOLUME, 200000); // E5
}

void playGameOverSound() {
    playSound(523.25, BGM_VOLUME, 200000); // C5
    playSound(440.00, BGM_VOLUME, 200000); // A4
    playSound(329.63, BGM_VOLUME, 200000); // E4
}

void playVictorySound() {
    playSound(783.99, BGM_VOLUME, 200000); // G5
    playSound(987.77, BGM_VOLUME, 200000); // B5
    playSound(1046.50, BGM_VOLUME, 200000); // C6
}

void playPelletSound() {
    playSound(659.25, BGM_VOLUME, 50000); // E5
}

void playPowerupSound() {
    playSound(784.00, BGM_VOLUME, 200000); // G5
}

void playGhostCollisionSound() {
    playSound(220.00, BGM_VOLUME, 200000); // A3
    playSound(164.81, BGM_VOLUME, 200000); // E3
}

void playBackgroundMusic() {
    if (bgm[bgm_index].frequency != 0) {
        playSound(bgm[bgm_index].frequency, BGM_VOLUME, bgm[bgm_index].duration_us);
    } else {
        usleep(bgm[bgm_index].duration_us);
    }
    bgm_index = (bgm_index + 1) % bgm_size;
}