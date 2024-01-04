document.addEventListener('DOMContentLoaded', function(){ // waits until content is loaded
    let rightAnswer = document.querySelector('#buttonCorrect');
    rightAnswer.addEventListener('click', function() {
        rightAnswer.style.backgroundColor = 'green'; //waits for the right answer to be clicked and turns it green/displays Correct if so
        document.querySelector('#response1').innerHTML = 'Correct!';
    });
    let wrongAnswers = document.querySelectorAll('#buttonWrong');
    for (let i = 0; i < wrongAnswers.length; i++) { //loops through all the incorrect answers
        wrongAnswers[i].addEventListener('click', function() {
            wrongAnswers[i].style.backgroundColor = 'red';
            document.querySelector('#response1').innerHTML = 'Incorrect'; //if it comes across a clicked incorrect answer, turn it red/display Incorrect
        });
    }
});
