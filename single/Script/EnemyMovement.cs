using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class EnemyMovement : MonoBehaviour
{
  public bool isPlayerInRange = false;    // Whether player is within the range and can be attacked.
  public float speed = 5f;
  public float rotateSpeed = 30f; // Angular speed in degrees per sec.

  Transform player;               // Reference to the player's position.
  EnemyShooting enemyShooting;    // Reference to this enemy's shooting.
  NavMeshAgent nav;               // Reference to the nav mesh agent

  void Start()
  {
    player = GameObject.FindGameObjectWithTag("Player").transform;
    nav = GetComponent<NavMeshAgent>();
    enemyShooting = GetComponent<EnemyShooting>();
    if (enemyShooting != null)
    {
      nav.stoppingDistance = enemyShooting.distanceToAttack;
      Debug.Log("Yes");
    }
    else
    {
      nav.stoppingDistance = 20;
    }

    nav.speed = speed;
  }

  // Update is called once per frame
  void Update()
  {
    if (Vector3.Distance(transform.position, player.position) < nav.stoppingDistance)
    {
      Vector3 direction = player.position - transform.position;
      direction.y = 0; // Ignore Y
      Quaternion targetRotation = Quaternion.LookRotation(direction);

      // The step size is equal to speed times frame time.
      var step = rotateSpeed * Time.deltaTime;

      // Rotate our transform a step closer to the target's
      transform.rotation = Quaternion.RotateTowards(transform.rotation, targetRotation, step);
      //transform.LookAt(player);
      isPlayerInRange = true;
    }
    else
    {
      nav.SetDestination(player.position);
      isPlayerInRange = false;
    }
  }
}
