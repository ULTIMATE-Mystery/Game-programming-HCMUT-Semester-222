using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyBomb : MonoBehaviour
{
  public float timeBetweenShootings = 2f;     // The time in seconds between each attack.
  public int damagePerShot = 10;               // The amount of health taken away per attack.
  public float distanceToAttack = 15f;

  public GameObject bullet;

  GameObject player;                          // Reference to the player GameObject.
  PlayerHealth playerHealth;                  // Reference to the player's health.
  EnemyHealth enemyHealth;                    // Reference to this enemy's health.
  EnemyMovement enemyMovement;                // Reference to this enemy's movement
  float timer;                                // Timer for counting up to the next attack.

  void Start()
  {
    // Setting up the references.
    player = GameObject.FindGameObjectWithTag("Player");
    enemyHealth = GetComponent<EnemyHealth>();
    enemyMovement = GetComponent<EnemyMovement>();
    playerHealth = player.GetComponent<PlayerHealth>();
  }

  // Update is called once per frame
  void Update()
  {
    // Add the time since Update was last called to the timer.
    timer += Time.deltaTime;

    if (enemyMovement == null)
    {
      Debug.Log("move");
    }
    if (enemyHealth == null)
    {
      Debug.Log("health");
    }
    // If the timer exceeds the time between attacks, the player is in range and this enemy is alive...
    if (timer >= timeBetweenShootings && enemyMovement.isPlayerInRange && enemyHealth.currentHealth > 0)
    {
      Attack();
    }
  }

  void Attack()
  {
    // Reset the timer.
    timer = 0f;

    // If the player has health to lose...
    if (playerHealth.currentHealth > 0)
    {
      Instantiate(bullet, player.transform.position, Quaternion.Euler(90, 0, 0));
      Debug.Log("Fire2");
    }
  }
}
