#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
float velocityX = -2;
float velocityY;
float maxSpeedBall = 5;
float leftPlayerY;
float rightPlayerY = 1;
int scoreLeft = 0;
int scoreRight = 0;
const float aiSpeed = 2.0f; // Maximale Geschwindigkeit der KI
sf::Font font("assets\\8bitOperatorPlus8-Regular.ttf");
sf::Text textLeft(font);
sf::Text textRight(font);
sf::CircleShape ball(10.f);
sf::RectangleShape leftPlayer;
sf::RectangleShape rightPlayer;
sf::Vector2f positionBall(300 , 300);
sf::Vector2f leftPlayerPos(0, 260);
sf::Vector2f rightPlayerPos(590, 260);

void handleInput() {
//LeftPlayer Keys
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) leftPlayerPos.y -= 5;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) leftPlayerPos.y += 5;
}

//Checkt Kollision vom Ball mit Spielern und Rand
void collisionBall() {

    //Checkt Rand
    if (positionBall.y < 0 || positionBall.y > 580) velocityY *= -1;
    //Checkt linken Player
    if (positionBall.x < leftPlayerPos.x + 10 && positionBall.y > leftPlayerPos.y - 10 && positionBall.y < leftPlayerPos.y + 90) {
        float intersectY = (ball.getPosition().y + ball.getRadius()) - (leftPlayer.getPosition().y + leftPlayer.getSize().y / 2); //Berechnung des Aufschlagpunkt
        float normalizedIntersectY = intersectY / (leftPlayer.getSize().y / 2); //Normalisierung
        velocityY = maxSpeedBall * normalizedIntersectY;
        velocityX = (velocityX < 0 ? -1 : 1) * std::sqrt(maxSpeedBall * maxSpeedBall - velocityY * velocityY); // Behalte konstante Geschwindigkeit
        velocityX *= -1;
    };
    //Checkt rechten Player
    if (positionBall.x > rightPlayerPos.x - (ball.getRadius() * 2) && positionBall.y > rightPlayerPos.y - 10 && positionBall.y < rightPlayerPos.y + 90) {
        //Versuch über die Bewegung des Spielers die diagonale Bewegung des Balls zu verändern
        float intersectY = (ball.getPosition().y + ball.getRadius()) - (rightPlayer.getPosition().y + rightPlayer.getSize().y / 2); //Berechnung des Aufschlagpunkt
        float normalizedIntersectY = intersectY / (rightPlayer.getSize().y / 2); // Normalisierung
        velocityY = maxSpeedBall * normalizedIntersectY;
        velocityX = (velocityX < 0 ? -1 : 1) * std::sqrt(maxSpeedBall * maxSpeedBall - velocityY * velocityY); // Behalte konstante Geschwindigkeit
        velocityX *= -1;
    }
}

//Checkt Kollision von beiden Spielern mit Rand
void collisionPlayer() {
    if (leftPlayerPos.y < 0) leftPlayerPos.y = 0;
    if (leftPlayerPos.y > 520) leftPlayerPos.y = 520;
    if (rightPlayerPos.y < 0) rightPlayerPos.y = 0;
    if (rightPlayerPos.y > 520) rightPlayerPos.y = 520;
}

void resetGame(bool rPressed) {

    //Ball zurücksetzen
    positionBall = sf::Vector2f(300, 300);
    ball.setPosition(positionBall);
    velocityX = -2;
    velocityY = 0;

    if (rPressed == true) {
    //Spieler zurücksetzen
    leftPlayer.setPosition(leftPlayerPos);
    rightPlayer.setPosition(rightPlayerPos);
    textLeft.setString("0");
    textRight.setString("0");
    }
}

void checkScore() {
    if (positionBall.x < 0) {
        scoreRight += 1;
        textRight.setString(std::to_string(scoreRight));
        resetGame(false);
    }
    if (positionBall.x > 600) {
        scoreLeft += 1;
        textLeft.setString(std::to_string(scoreLeft));
        resetGame(false);
    }
}

void updateAI() {
    float ballCenter = positionBall.y + ball.getRadius();
    float paddleCenter = rightPlayerPos.y + rightPlayer.getSize().y / 2;

    // KI bewegt sich nur, wenn der Ball nicht schon auf gleicher Höhe ist
    if (ballCenter < paddleCenter) {
        rightPlayerPos.y -= aiSpeed; // Nach oben bewegen
    }
    if (ballCenter > paddleCenter) {
        rightPlayerPos.y += aiSpeed; // Nach unten bewegen
    }
}
int main()
{
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Simple Pong"); // Window Initialisierung
    window.setFramerateLimit(60); //Framelimit
   
    //Spielball
    ball.setPosition(positionBall);              
    //Linker Spieler
    leftPlayer.setSize(sf::Vector2f(10, 80));
    leftPlayer.setPosition(leftPlayerPos);
    //Rechter Spieler
    rightPlayer.setSize(sf::Vector2f(10, 80));
    rightPlayer.setPosition(rightPlayerPos);

    //Score Text links und rechts
    textLeft.setString("0");
    textLeft.setCharacterSize(50);
    textLeft.setPosition(sf::Vector2f(100, 30));
    textRight.setString("0");
    textRight.setCharacterSize(50);
    textRight.setPosition(sf::Vector2f(500, 30));


    while (window.isOpen()) //Event-Loop
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close(); //Close with X
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close(); //Close with escape 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) resetGame(true); //Restard with R
        }
        //Bewegung verarbeiten
        handleInput();
        //Kollisionschecks
        collisionBall();
        collisionPlayer();
        //Check ob Ball außerhalb von Spielfeld;
        checkScore();
        
        //AI des rechten Spielers
        updateAI();

        //Positionen setzen
        leftPlayer.setPosition(leftPlayerPos);
        rightPlayer.setPosition(rightPlayerPos);

        //Movement Ball
        positionBall.x += velocityX;
        positionBall.y += velocityY;
        ball.setPosition(positionBall);
    

        window.clear();
        window.draw(ball);
        window.draw(leftPlayer);
        window.draw(rightPlayer);
        window.draw(textLeft);
        window.draw(textRight);
        window.display();
    }

}

