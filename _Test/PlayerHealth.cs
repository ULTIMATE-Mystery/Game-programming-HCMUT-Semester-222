using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerHealth : MonoBehaviour
{
  public int startingHealth = 100;                            // The amount of health the player starts the game with.
  public int currentHealth;                                   // The current health the player has.
  /*
	public Slider healthSlider;                                 // Reference to the UI's health bar.
  public Image damageImage;                                   // Reference to an image to flash on the screen on being hurt.
	public AudioClip deathClip;                                 // The audio clip to play when the player dies.
	public float flashSpeed = 5f;                               // The speed the damageImage will fade at.
  public Color flashColour = new Color(1f, 0f, 0f, 0.1f);     // The colour the damageImage is set to, to flash.
	*/

  PlayerMovementOff playerMovement;                            // Reference to the player's movement.
  /*
	PlayerShooting playerShooting;                              // Reference to the PlayerShooting script.
	*/
  bool isDamaged;                                             // Indicate Wheather the player gets damaged.
  bool isDead;                                                // Indicate whether the player is dead.
  bool isInvulnerable;                                        // Indicates whether the player cannot be damaged.
  public HealthBarHud healthBar;

  void Start()
  {
    playerMovement = GetComponent<PlayerMovementOff>();
    //playerShooting = GetComponent<PlayerShooting>();

    currentHealth = startingHealth;
    healthBar.SetMaxHealth(startingHealth);
  }

  void Update()
  {
    // if (isDamaged)
    // {
    //   // ... set the colour of the damageImage to the flash colour.
    //   damageImage.color = flashColour;
    // }
    // else
    // {
    //   // ... transition the colour back to clear.
    //   damageImage.color = Color.Lerp(damageImage.color, Color.clear, flashSpeed * Time.deltaTime);
    // }

    isDamaged = false;
  }

  public void TakeDamage(int amount)
  {
    if (isInvulnerable)
    {
      return;
    }

    // Set the damaged flag so the screen will flash.
    isDamaged = true;

    // Reduce the current health by the damage amount.
    currentHealth -= amount;

    healthBar.SetHealth(currentHealth);

    // Set the health bar's value to the current health.
    //healthSlider.value = currentHealth;

    // Play the hurt sound effect.
    //playerAudio.Play();

    // If the player has lost all it's health and the death flag hasn't been set yet...
    if (currentHealth <= 0 && !isDead)
    {
      Death();
    }
  }

  void Death()
  {
    // Set the death flag so this function won't be called again.
    isDead = true;

    playerMovement.enabled = false;
    //playerShooting.enabled = false;
  }
}
