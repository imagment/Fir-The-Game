import java.util.ArrayList;

class Ball {
  PVector position;
  PVector velocity;
  float radius;
  int type;

  Ball(PVector pos, PVector vel, float rad, int t) {
    position = pos;
    velocity = vel;
    radius = rad;
    type = t;
  }
  
  void update() {
    position.add(velocity);
    velocity.mult(0.99);
    checkWallCollisions();
  }
  
  void draw() {
    noStroke();
    float angleStep = TWO_PI / 8;

    if (type == 4) {
      fill(0, 0, 255);
      ellipse(position.x, position.y, radius * 2, radius * 2);
    } else {
      for (int i = 0; i < 8; i++) {
        float startAngle = i * angleStep;
        float endAngle = startAngle + angleStep;
        if (i < (8 / type)) {
          fill(0, 0, 255);
        } else {
          fill(150);
        }
        arc(position.x, position.y, radius * 2, radius * 2, startAngle, endAngle, PIE);
      }
    }
  }
  
  void checkWallCollisions() {
    if (position.x < radius || position.x > width - radius) {
      velocity.x *= -1;
      position.x = constrain(position.x, radius, width - radius);
    }
    if (position.y < radius || position.y > height - radius) {
      velocity.y *= -1;
      position.y = constrain(position.y, radius, height - radius);
    }
  }
  
  void checkCollision(Ball other) {
    float distance = PVector.dist(position, other.position);
    if (distance < radius + other.radius) {
      if (type != other.type) {
        velocity.set(0, 0);
        other.velocity.set(0, 0);
      } else {
        if (random(1) < 0.5) {
          boolean allSameType = true;
          for (Ball ball : balls) {
            if (ball.type != this.type) {
              allSameType = false;
              break;
            }
          }

          if (allSameType) {
            ArrayList<Ball> mergedBalls = new ArrayList<Ball>();
            mergedBalls.add(new Ball(
              new PVector(width / 2, height / 2),
              new PVector(0, 0),
              20,
              1
            ));

            balls = mergedBalls;
          } else {
            radius = 20;
            other.radius = 20;
            velocity.set(0, 0);
            other.velocity.set(0, 0);
          }
        }
      }
    }
  }
}

PVector[] goals;
float goalRadius = 15;
int currentRedGoal = 0;
boolean pauseRedGoalChange = false;
int pauseTime = 0;

ArrayList<Ball> balls;
int currentBallIndex = 0;
boolean isDragging = false;
PVector dragStart;
PVector dragEnd;

void setup() {
  size(800, 400);
  dragStart = new PVector();
  dragEnd = new PVector();
  
  goals = new PVector[6];
  goals[0] = new PVector(0, 0);
  goals[1] = new PVector(width, 0);
  goals[2] = new PVector(0, height / 2);
  goals[3] = new PVector(width, height / 2);
  goals[4] = new PVector(0, height);
  goals[5] = new PVector(width, height);
  
  balls = new ArrayList<Ball>();
  balls.add(new Ball(new PVector(width / 2, height / 2), new PVector(0, 0), 20, 1));
  
  frameRate(60);
}

void draw() {
  background(0, 100, 0);

  for (int i = 0; i < goals.length; i++) {
    if (i == currentRedGoal) {
      fill(255, 0, 0);
    } else {
      fill(0);
    }
    ellipse(goals[i].x, goals[i].y, goalRadius * 2, goalRadius * 2);
  }

  for (Ball ball : balls) {
    ball.update();
    ball.draw();
  }
  
  if (isDragging) {
    stroke(255);
    line(dragStart.x, dragStart.y, dragEnd.x, dragEnd.y);
  }

  for (int i = 0; i < goals.length; i++) {
    for (Ball ball : balls) {
      if (PVector.dist(ball.position, goals[i]) < ball.radius + goalRadius) {
        if (i == currentRedGoal) {
          ball.position.set(width / 2, height / 2);
          ball.velocity.set(0, 0);
          pauseRedGoalChange = false;
        } else {
          pauseRedGoalChange = true;
          pauseTime = millis();
        }
      }
    }
  }

  if (!pauseRedGoalChange) {
    if (frameCount % 60 == 0) {
      currentRedGoal = (currentRedGoal + 1) % goals.length;
    }
  } else {
    if (millis() - pauseTime > 1000) {
      pauseRedGoalChange = false;
    }
  }
}

void mousePressed() {
  if (mouseButton == RIGHT) {
    // Removed splitBall() function call
  } else if (!isDragging) {
    for (int i = 0; i < balls.size(); i++) {
      if (PVector.dist(balls.get(i).position, new PVector(mouseX, mouseY)) < balls.get(i).radius) {
        currentBallIndex = i;
        dragStart.set(mouseX, mouseY);
        isDragging = true;
        break;
      }
    }
  }
}

void mouseDragged() {
  if (isDragging) {
    dragEnd.set(mouseX, mouseY);
  }
}

void mouseReleased() {
  if (isDragging) {
    PVector force = PVector.sub(dragEnd, dragStart).mult(0.1);
    balls.get(currentBallIndex).velocity.add(force);
    isDragging = false;
  }
}
