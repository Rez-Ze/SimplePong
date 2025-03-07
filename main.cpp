#include <SFML/Graphics.hpp>
#include <math.h>

float velocityX = -1;
float velocityY;
float maxSpeedBall = 5;
float leftPlayerY;
float rightPlayerY = 1;
sf::CircleShape ball(10.f);
sf::RectangleShape leftPlayer;
sf::RectangleShape rightPlayer;
sf::Vector2f positionBall(300 , 300);
sf::Vector2f leftPlayerPos(0, 300);
sf::Vector2f rightPlayerPos(590, 300);

void handleInput() {
//LeftPlayer Keys
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) leftPlayerPos.y -= 5;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) leftPlayerPos.y += 5;
}

//Checkt collision vom Ball mit Spielern und Rand
void collisionBall() {

    //Checkt Rand
    if (positionBall.y < 0 || positionBall.y > 580) velocityY *= -1;
    //Checkt linken Player
    if (positionBall.x < leftPlayerPos.x + 10 && positionBall.y > leftPlayerPos.y - 10 && positionBall.y < leftPlayerPos.y + 90) {
        float intersectY = (ball.getPosition().y + ball.getRadius()) - (leftPlayer.getPosition().y + leftPlayer.getSize().y / 2);
        float normalizedIntersectY = intersectY / (leftPlayer.getSize().y / 2);
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

//Checkt collision von beiden Spielern mit Rand
void collisionPlayer() {
    if (leftPlayerPos.y < 0 || leftPlayerPos.y > 520) leftPlayerY *= -1;
    if (rightPlayerPos.y < 0 || rightPlayerPos.y > 520) rightPlayerY *= -1;
}

//wenn R gedrückt wird, wird Spiel resetet
void resetGame() {
    //Ball zurücksetzen
    positionBall = sf::Vector2f(300, 300);
    ball.setPosition(positionBall);
    velocityX = -1;
    velocityY = 0;

    //Spieler zurücksetzen
    leftPlayerPos = sf::Vector2f(0, 300);
    rightPlayerPos = sf::Vector2f(590, 300);
    leftPlayer.setPosition(leftPlayerPos);
    rightPlayer.setPosition(rightPlayerPos);
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

   

    while (window.isOpen()) //Event-Loop
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close(); //Close with X
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close(); //Close with escape 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) resetGame();
        }
        //Bewegung verarbeiten
        handleInput();
        //Methodenaufruf von Collision
        collisionBall();
        collisionPlayer();

        //Movement linker Spieler
        leftPlayer.setPosition(leftPlayerPos);

        //Movement rechter Spieler
        rightPlayerPos.y = positionBall.y-40;
        rightPlayer.setPosition(rightPlayerPos);

        //Movment Ball
        positionBall.x += velocityX;
        positionBall.y += velocityY;
        ball.setPosition(positionBall);
    

        window.clear();
        window.draw(ball);
        window.draw(leftPlayer);
        window.draw(rightPlayer);
        window.display();
    }

}

