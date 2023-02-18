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
import random
from pygame import *
from Classes.GameDefine import Constants
from Classes.GameDefine import Zombie
from Classes.SoundEffect import SoundEffect

class Game:
    def __init__(self):
        # Initialize screen and title
        self.screen = pygame.display.set_mode((Constants.SCREEN_WIDTH, Constants.SCREEN_HEIGHT))
        pygame.display.set_caption(Constants.GAME_TITLE)
        self.background = pygame.image.load(Constants.IMAGE_BG)
        self.gameover = pygame.image.load(Constants.IMAGE_GAMEOVER)

        # Font for displaying text
        self.font_obj = pygame.font.Font(Constants.FONT_NAME, Constants.FONT_SIZE)

        # Initialize game statistics
        self.hits = 0
        self.misses = 0
        self.level = 1
        self.brains = 3
        self.zombie_count = 0

        # Initialize zombie list
        self.zombie = []
        for zombieIndex in range(Constants.GRAVE_NUM_MAX):
            temp = Zombie()
            self.zombie.append(temp)

        # Position of the graves in background
        self.grave_positions = []
        self.grave_positions.append(Constants.GRAVE_POS_1)
        self.grave_positions.append(Constants.GRAVE_POS_2)
        self.grave_positions.append(Constants.GRAVE_POS_3)
        self.grave_positions.append(Constants.GRAVE_POS_4)
        self.grave_positions.append(Constants.GRAVE_POS_5)
        self.grave_positions.append(Constants.GRAVE_POS_6)
        self.grave_positions.append(Constants.GRAVE_POS_7)
        self.grave_positions.append(Constants.GRAVE_POS_8)
        self.grave_positions.append(Constants.GRAVE_POS_9)
        self.grave_positions.append(Constants.GRAVE_POS_10)

        # Initialize zombie's sprite sheet - 6 states    
        zombie_sprite_sheet = pygame.image.load(Constants.IMAGE_ZOMBIE)
        self.zombie_image = []
        self.zombie_image.append(zombie_sprite_sheet.subsurface(Constants.ZOM_SPRITE_1))
        self.zombie_image.append(zombie_sprite_sheet.subsurface(Constants.ZOM_SPRITE_2))
        self.zombie_image.append(zombie_sprite_sheet.subsurface(Constants.ZOM_SPRITE_3))
        self.zombie_image.append(zombie_sprite_sheet.subsurface(Constants.ZOM_SPRITE_4))
        self.zombie_image.append(zombie_sprite_sheet.subsurface(Constants.ZOM_SPRITE_5))
        self.zombie_image.append(zombie_sprite_sheet.subsurface(Constants.ZOM_SPRITE_6))

        # Initialize hammer image
        self.hammer_image = pygame.image.load(Constants.IMAGE_HAMMER).convert_alpha()
        self.hammer_image_rotate = transform.rotate(self.hammer_image.copy(), Constants.HAMMER_ANGLE)

        # Initialize sound effects
#        self.soundEffect = SoundEffect()

        # Initialize brains
        self.brain_image = pygame.transform.scale(pygame.image.load(Constants.IMAGE_BRAIN), (40, 35))

    # Calculate the level up based on current hit & LEVEL_HIT_GAP
    def getPlayerLevel(self):
        if self.level >= Constants.LEVEL_CAP:
            return Constants.LEVEL_CAP
        nextLevel = int(self.hits / Constants.LEVEL_UP_GAP) + 1
        if nextLevel != self.level:         
#            self.soundEffect.playLevelUpSound() # Play sound level up
            self.brains += 1
        return nextLevel

    # Calculate the time respawning new zombie
    def getStayTime(self, maxStayTime):
        maxStayTime = Constants.STAY_TIME - self.level * Constants.STAY_DELTA_TIME
        if maxStayTime <= Constants.STAY_DELTA_TIME:
            maxStayTime = Constants.STAY_DELTA_TIME
        return maxStayTime

    # Check if the mouse hits zombie or not
    def isZombieHit(self, mouse_position, current_grave_position):
        mouse_x = mouse_position[0]
        mouse_y = mouse_position[1]
        current_hole_x = current_grave_position[0]
        current_hole_y = current_grave_position[1]
        distanceX = mouse_x - current_hole_x
        distanceY = mouse_y - current_hole_y
        if (0 < distanceX < Constants.ZOM_WIDTH) and (0 < distanceY < Constants.ZOM_HEIGHT):
            return True
        else:
            return False
    
    # Generate zombie through time based on player's level
    def generateZombie(self, gameTime, lastSpawnTime, respawnTime):
        if self.zombie_count > Constants.GRAVE_NUM_MAX:
            return 0
        if gameTime - lastSpawnTime < respawnTime:
            return 0
        spawnIndex = random.randint(0, Constants.GRAVE_NUM_MAX - self.zombie_count)
        for zombieIndex in range(Constants.GRAVE_NUM_MAX):
            if self.zombie[zombieIndex].zombieStatus == -1:
                spawnIndex -= 1
            if spawnIndex == -1:
                self.zombie[zombieIndex].zombieStatus = 0
                self.zombie[zombieIndex].pic = self.zombie_image[0]
                self.zombie_count += 1
        return 1
    
    # Calculate player's brains
    def update_brain(self, isEaten):
        if isEaten:
            self.brains -= 1
        for brainIndex in range(self.brains):
            self.screen.blit(self.brain_image, (710 - brainIndex * 60, 550))

    # Update, rotate the hammer
    def update_hammer(self, mouse_position, image, image_rotate, isClicked):
        mouse_x = mouse_position[0] - Constants.HAMMER_DISTANCE_X   
        mouse_y = mouse_position[1] - Constants.HAMMER_DISTANCE_Y
        if isClicked:
            self.screen.blit(image_rotate, [mouse_x, mouse_y])
        else:
            self.screen.blit(image, [mouse_x, mouse_y])

    # Update zombie animation
    def update_sprite(self):
        self.screen.blit(self.background, (0, 0))
        for zombieIndex in range(Constants.GRAVE_NUM_MAX):
            if self.zombie[zombieIndex].zombieStatus == -1:
                continue
            self.screen.blit(self.zombie[zombieIndex].pic, (self.grave_positions[zombieIndex]))

    # Update player's hits, misses, level, brains
    def update_statistics(self, isClicked, isEaten):
        # Update the zombie animation
        #self.screen.blit(self.background, (0, 0))
        #self.screen.blit(image, (self.grave_positions[graveStoneIndex]))
        self.update_hammer(mouse.get_pos(), self.hammer_image, self.hammer_image_rotate, isClicked)
        self.update_brain(isEaten)

        # Update the player's hits
        current_hit_string = Constants.HIT_TEXT + str(self.hits)
        hit_text = self.font_obj.render(current_hit_string, True, Constants.TEXT_COLOR)
        hit_text_pos = hit_text.get_rect()
        hit_text_pos.centerx = Constants.HIT_POS
        hit_text_pos.centery = Constants.FONT_SIZE
        self.screen.blit(hit_text, hit_text_pos)

        # Update the player's misses
        current_misses_string = Constants.MISS_TEXT + str(self.misses)
        misses_text = self.font_obj.render(current_misses_string, True, Constants.TEXT_COLOR)
        misses_text_pos = misses_text.get_rect()
        misses_text_pos.centerx = Constants.MISS_POS
        misses_text_pos.centery = Constants.FONT_SIZE
        self.screen.blit(misses_text, misses_text_pos)

        # Update the player's level
        current_level_string = Constants.LEVEL_TEXT + str(self.level)
        level_text = self.font_obj.render(current_level_string, True, Constants.TEXT_COLOR)
        level_text_pos = level_text.get_rect()
        level_text_pos.centerx = Constants.LEVEL_POS
        level_text_pos.centery = Constants.FONT_SIZE
        self.screen.blit(level_text, level_text_pos)

    # Start the game's main loop
    def start(self):
        # Game settings
        loop = True
        pic = self.zombie_image[0]
        mouse.set_visible(False)

        # Flag variables
        isHit = False
        isClicked = False
        is_down = False
        isEaten = False

        # Time variables
        clock = pygame.time.Clock()     # Time variables
        cycle_time = 0                  # Count clock's time
        gameTime = 0
        maxStayTime = 1.5
        stayTime = 0
        hammer_time = 0
        lastSpawnTime = 0

        # Zombie-spawning variables
        zombieStatus = 1
        graveStoneIndex = 0
        spawnAnimationIndex = 0
        deadAnimationIndex = 3
        animationIndex = -1

        for i in range(len(self.zombie_image)):
            self.zombie_image[i].set_colorkey((0, 0, 0))
            self.zombie_image[i] = self.zombie_image[i].convert_alpha()     

        while loop:
            # Calculate game input   
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    loop = False
                if event.type == MOUSEBUTTONDOWN and event.button == Constants.LEFT_MOUSE_BUTTON:
                    isClicked = True
                    if self.isZombieHit(mouse.get_pos(), self.grave_positions[graveStoneIndex]):
                        zombieStatus = 3
                        is_down = False
                        self.hits += 1
                        self.level = self.getPlayerLevel()
#                        self.soundEffect.playHitSound() # Play hit sound effect
                        isHit = True
                    else:
                        self.misses += 1
#                        self.soundEffect.playMissSound() # Play miss sound effect
                        isHit = False
                else:
                    isClicked = False

            # Calculate game time
            mil = clock.tick(Constants.FPS)
            sec = mil / 1000.0
            cycle_time += sec
            gameTime += sec
#            print(sec, ' ', gameTime)
            print(self.zombie[0].zombieStatus, ' ', self.zombie[0].animationIndex)
#            maxStayTime = self.getStayTime(maxStayTime)
            
            for zombieIndex in range(Constants.GRAVE_NUM_MAX):
                thisZombie = self.zombie[zombieIndex]
                if thisZombie.zombieStatus == -1:
                    continue
                thisZombie.stayTime += sec

                # Zombie status: rise
                if thisZombie.zombieStatus == 0:
                    if  thisZombie.stayTime > Constants.SPAWN_ANI_TIME:
                        if thisZombie.animationIndex > Constants.SPAWN_ANI_INDEX_MAX:
                            if thisZombie.stayTime > maxStayTime:
                                thisZombie.animationIndex = Constants.SPAWN_ANI_INDEX_MAX
                                thisZombie.zombieStatus = 1
                                thisZombie.stayTime = 0
                        else:
                            thisZombie.pic = self.zombie_image[thisZombie.animationIndex]
                            thisZombie.animationIndex += 1
                            thisZombie.stayTime = 0

                # Zombie status: run away
                if thisZombie.zombieStatus == 1:
                    if thisZombie.stayTime > Constants.SPAWN_ANI_TIME:
                        thisZombie.pic = self.zombie_image[thisZombie.animationIndex]
                        thisZombie.animationIndex -= 1
                        thisZombie.stayTime = 0
                        if thisZombie.animationIndex < 0:
                            thisZombie.zombieStatus = -1
                            isEaten = True

                # Zombie status: dead
                if thisZombie.zombieStatus == 2:
                    if thisZombie.stayTime > Constants.DEAD_ANI_TIME:
                        thisZombie.pic = self.zombie_image[thisZombie.animationIndex]
                        thisZombie.animationIndex += 1
                        thisZombie.stayTime = 0
                        if thisZombie.animationIndex > Constants.DEAD_ANI_INDEX_MAX:
                            thisZombie.zombieStatus = -1

            self.update_sprite()
            self.update_statistics(isClicked, isEaten)
            if self.generateZombie(gameTime, lastSpawnTime, 3) == 1:
                lastSpawnTime = gameTime

            # Gameover condition
            if self.brains <= 0:
                self.screen.blit(self.gameover, (0, 0))
                mouse.set_visible(True)
            else:
                isEaten = False
   
            # Update the display     
            pygame.display.flip()

###########################################################################
# Initialize the game
pygame.mixer.init(frequency=22050, size=-16, channels=2, buffer=512)
pygame.init()

# Start game - Run main loop
myGame = Game()
myGame.start()

# Exit game if the main loop ends
pygame.quit()