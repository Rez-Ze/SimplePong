#include <SFML/Graphics.hpp>

float velocityX = -1;
float velocityY;
float leftPlayerY;
float rightPlayerY = 1;
sf::Vector2f positionBall(300 , 300);
sf::Vector2f leftPlayerPos(0, 300);
sf::Vector2f rightPlayerPos(590, 300);

//Checkt collision vom Ball mit Spielern und Rand
void collisionBall() {

    //Checkt Rand
    if (positionBall.y < 0 || positionBall.y > 580) velocityY *= -1;
    //Checkt linken Player
    if (positionBall.x < leftPlayerPos.x + 10 && positionBall.y > leftPlayerPos.y - 10 && positionBall.y < leftPlayerPos.y + 90) {
        //Versuch über die Bewegung des Spielers die diagonale Bewegung des Balls zu verändern
        if (leftPlayerY < 0 ) velocityY += -leftPlayerY;
        else if(leftPlayerY > 0) velocityY += leftPlayerY;
        else velocityY = 0;
        velocityX *= -1;
    };
    //Checkt rechten Player
    if (positionBall.x > rightPlayerPos.x - 10 && positionBall.y > rightPlayerPos.y - 10 && positionBall.y < rightPlayerPos.y + 90) {
        //Versuch über die Bewegung des Spielers die diagonale Bewegung des Balls zu verändern
        if (rightPlayerY < 0 ) velocityY += -rightPlayerY;
        else if(rightPlayerY > 0) velocityY += rightPlayerY;
        else velocityY = 0;
        velocityX *= -1;
    }
}

//Checkkt collision von beiden Spielern mit Rand
void collisionPlayer() {
    if (leftPlayerPos.y < 0 || leftPlayerPos.y > 520) leftPlayerY *= -1;
    if (rightPlayerPos.y < 0 || rightPlayerPos.y > 520) rightPlayerY *= -1;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({600, 600}), "SFML works!"); // Window Initialisierung
    window.setFramerateLimit(60); //Framelimit
   
    //Spielball
    sf::CircleShape ball(10.f);
    ball.setPosition(positionBall);              
    ball.setFillColor(sf::Color::White);

    //Linker Spieler
    sf::RectangleShape leftPlayer;
    leftPlayer.setSize(sf::Vector2f(10, 80));
    leftPlayer.setPosition(leftPlayerPos);
    //Rechter Spieler
    sf::RectangleShape rightPlayer;
    rightPlayer.setSize(sf::Vector2f(10, 80));
    rightPlayer.setPosition(rightPlayerPos);

   

    while (window.isOpen()) //Event-Loop
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close(); //Close with X
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close(); //Close with escape 

            //LeftPlayer Keys
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) leftPlayerY -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) leftPlayerY += 1;
        }
        
        //Methodenaufruf von Collision
        collisionBall();
        collisionPlayer();

        //Movement linker Spieler
        leftPlayerPos.y +=leftPlayerY;
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

