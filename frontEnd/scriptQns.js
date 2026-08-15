let questions = [];
let content = "";
let qnNo = 0;

document.getElementById("addButton").onclick = addQuestion;
document.getElementById("saveButton").onclick = saveFunction;

function addQuestion() {
    let questionTemp = {
        question:document.getElementById("questionBox").value,
        options:[
            document.getElementById("optionText1").value,
            document.getElementById("optionText2").value,
            document.getElementById("optionText3").value,
            document.getElementById("optionText4").value,
        ],
        correctOption:0
    };
    for(let i = 2; i < 5; i++) {      
        if(document.getElementById(`option${i}`).checked) {
            questionTemp.correctOption = i - 1;
            break;
        }
    }
    questions.push(questionTemp);
    updateThumb(questionTemp.question);

    document.getElementById("questionBox").value = "";
    document.getElementById("optionText1").value = "";
    document.getElementById("optionText2").value = "";
    document.getElementById("optionText3").value = "";
    document.getElementById("optionText4").value = "";

    document.querySelectorAll('input[name="options"]').forEach(radio => {
        radio.checked = false;
    });
}

function updateThumb(question) {
    qnNo++;
    content += `<p>${qnNo}. ${question}</p>`;
    document.getElementById("questionThumb").innerHTML = content;
}
function saveFunction() {
    let finalQuestions = [];
    finalQuestions[0] = {title:document.getElementById("titleBox").value};
    finalQuestions[1] = questions;

    sessionStorage.setItem("quizQuestions", JSON.stringify(finalQuestions));
    window.location.href="ViewSurvey";
    
}




