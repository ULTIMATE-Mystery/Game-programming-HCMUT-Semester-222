##############################################
#   Assignment 1 - Game Programming - Semester 222
#
#   Group:
#   Phạm Duy Quang - 2011899
#   Nguyễn Vương Long - 1911520
#   Trương Tuấn Cường - 2012775
#   Nguyễn Duy Hà - 2011130
##############################################

class GameConstants:
	SCREEN_WIDTH = 800
	SCREEN_HEIGHT = 600
	FPS = 60

class LevelConstants:
	LEVEL_UP_GAP = 5
	LEVEL_DELAY_TIME = 5

class ZombieConstants:
	ZOM_WIDTH = 98
	ZOM_HEIGHT = 81

	ZOM_SPRITE_1 = [179, 0, 117, 81]
	ZOM_SPRITE_2 = [313, 0, 117, 81]
	ZOM_SPRITE_3 = [449, 0, 117, 81]
	ZOM_SPRITE_4 = [585, 0, 117, 81]
	ZOM_SPRITE_5 = [717, 0, 117, 81]
	ZOM_SPRITE_6 = [864, 0, 117, 81]

class GraveConstants:
	GRAVE_NUM_MAX = 10
	GRAVE_POS_1 = [101, 205 - ZombieConstants.ZOM_HEIGHT]
	GRAVE_POS_2 = [350, 204 - ZombieConstants.ZOM_HEIGHT]
	GRAVE_POS_3 = [580, 214 - ZombieConstants.ZOM_HEIGHT]
	GRAVE_POS_4 = [182, 297 - ZombieConstants.ZOM_HEIGHT]
	GRAVE_POS_5 = [422, 295 - ZombieConstants.ZOM_HEIGHT]
	GRAVE_POS_6 = [254, 404 - ZombieConstants.ZOM_HEIGHT]
	GRAVE_POS_7 = [505, 414 - ZombieConstants.ZOM_HEIGHT]
	GRAVE_POS_8 = [98, 514 - ZombieConstants.ZOM_HEIGHT]
	GRAVE_POS_9 = [348, 510 - ZombieConstants.ZOM_HEIGHT]
	GRAVE_POS_10 = [589, 510 - ZombieConstants.ZOM_HEIGHT]

class HammerConstants:
	HAMMER_ANGLE = 45
	HAMMER_DISTANCE_X = 16
	HAMMER_DISTANCE_Y = 25

class TimeConstants:
	SPAWN_ANI_TIME = 0.1
	DEAD_ANI_TIME = 0.1

	RESPAWN_TIME = 1.5
	RESPAWN_DELTA_TIME = 0.15

	HAMMER_ANI_TIME = 0.8

class AnimationConstants:	
	SPAWN_ANI_INDEX_MAX = 2
	DEAD_ANI_INDEX_MAX = 4

class FontConstants:
	FONT_NAME = "./Resources/fonts/ZOMBIE.ttf"
	FONT_SIZE = 36

class TextConstants:
	GAME_TITLE = "Whack A Zombie - Assignment 1"
	HIT_TEXT = "HITS - "
	MISS_TEXT = "MISSES - "
	LEVEL_TEXT = "LEVEL - "

	HIT_POS = GameConstants.SCREEN_WIDTH / 2
	MISS_POS = GameConstants.SCREEN_WIDTH / 5 * 4
	LEVEL_POS = GameConstants.SCREEN_WIDTH / 5

	TEXT_COLOR = [255, 255, 255] # White

class ImageConstants:
	IMAGE = "./Resources/images/"
	IMAGE_BG = IMAGE + "background.png"
	IMAGE_HAMMER = IMAGE + "hammer.png"
	IMAGE_ZOMBIE = IMAGE + "zombie.png"
 
class SoundConstants:
	SOUND = "./Resources/sounds/"
	SOUND_BG = SOUND + "music_bg.mp3"
	SOUND_HIT = SOUND + "hit.wav"
	SOUND_MISS = SOUND + "miss.wav"
	SOUND_LEVEL_UP = SOUND + "level_up.wav"

class Constants(GameConstants, LevelConstants, ZombieConstants, GraveConstants, HammerConstants, 
	TimeConstants, AnimationConstants, FontConstants, TextConstants, ImageConstants, SoundConstants):
	LEFT_MOUSE_BUTTON = 1