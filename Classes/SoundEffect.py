##############################################
#   Assignment 1 - Game Programming - Semester 222
#
#   Group:
#   Phạm Duy Quang - 2011899
#   Nguyễn Vương Long - 1911520
#   Trương Tuấn Cường - 2012775
#   Nguyễn Duy Hà - 2011130
##############################################

import pygame
from pygame import *
from Classes.GameDefine import SoundConstants

class SoundEffect:
    def __init__(self):
        self.bgMusic = pygame.mixer.music.load(SoundConstants.SOUND_BG)
        self.hitSound = pygame.mixer.Sound(SoundConstants.SOUND_HIT)
        self.missSound = pygame.mixer.Sound(SoundConstants.SOUND_MISS)
        self.levelUpSound = pygame.mixer.Sound(SoundConstants.SOUND_LEVEL_UP)
        pygame.mixer.music.play(-1)

    def playHitSound(self):
        self.hitSound.play()

    def playMissSound(self):
        self.missSound.play()

    def playLevelUpSound(self):
        self.levelUpSound.play()