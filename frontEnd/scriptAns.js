let questions;

window.onload = () => {
    questions = JSON.parse(sessionStorage.getItem("quizQuestions") || []);
    document.getElementById("title").textContent = questions[0].title;

    questions[1].forEach((question, index) => {
        printQuestion(question, index);
    }); 
}

// Mock data
// let questions = [
//   {"title": "Programming test"}, [ 
//     {
//       "question": "Which of these is NOT a programming paradigm?",
//       "options": ["Object-Oriented", "Functional", "Declarative", "Cyclomatic"],
//       "correctOption": 3
//     },
//     {
//       "question": "What does the 'O' stand for in SOLID principles?",
//       "options": ["Object", "Open/Closed", "Oriented", "Optimization"],
//       "correctOption": 1
//     },
//     {
//       "question": "Which data structure uses LIFO (Last In, First Out) principle?",
//       "options": ["Queue", "Stack", "Linked List", "Tree"],
//       "correctOption": 1
//     },
//     {
//       "question": "What is the time complexity of binary search on a sorted array?",
//       "options": ["O(n)", "O(n²)", "O(log n)", "O(1)"],
//       "correctOption": 2
//     },
//     {
//       "question": "Which of these is a compiled language?",
//       "options": ["Python", "JavaScript", "C++", "PHP"],
//       "correctOption": 2
//     },
//     {
//       "question": "What does HTTP stand for?",
//       "options": ["HyperText Transfer Protocol", "High Transfer Text Protocol", "Hyper Transfer Text Protocol", "HyperText Transmission Protocol"],
//       "correctOption": 0
//     },
//     {
//       "question": "In object-oriented programming, what is encapsulation?",
//       "options": ["Hiding implementation details and exposing only necessary features", "Creating multiple methods with same name", "Inheriting properties from parent class", "Converting one data type to another"],
//       "correctOption": 0
//     },
//     {
//       "question": "Which of these is a NoSQL database?",
//       "options": ["MySQL", "PostgreSQL", "MongoDB", "SQLite"],
//       "correctOption": 2
//     },
//     {
//       "question": "What is the main purpose of an index in a database?",
//       "options": ["To reduce storage space", "To improve query performance", "To encrypt data", "To create backups"],
//       "correctOption": 1
//     },
//     {
//       "question": "Which algorithm is used to find the shortest path in a weighted graph?",
//       "options": ["Depth-First Search", "Bubble Sort", "Dijkstra's Algorithm", "Binary Search"],
//       "correctOption": 2
//     }
//   ]
// ];
// document.getElementById("title").textContent = questions[0].title;

// questions[1].forEach((question, index) => {
//         printQuestion(question, index);
//     }); 

document.getElementById("checkButton").onclick = checkAnswers;

function printQuestion(questionObj, questionIndex) {
    let content = `<p>${questionObj.question}</p>`;

    for(let i = 0; i < 4; i++) {
        content += `<input type="radio" id="id${questionIndex}${i}" name="${questionIndex}">
                    <label for="id${questionIndex}${i}" id="label${questionIndex}${i}">${questionObj.options[i]}</label><br>`
    }
    content += `<br><hr>`
    document.getElementById("output").innerHTML += content;
}

function checkAnswers(){
    questions[1].forEach((question, index) => {
        document.getElementById(`label${index}${question.correctOption}`).style.color = "blue";
    });
}