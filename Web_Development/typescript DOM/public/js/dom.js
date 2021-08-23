"use strict";
var _a, _b, _c;
// Alternating the colours of the list elements
function backgroundColour() {
    var counter = 0;
    var listElements = document.querySelectorAll('li');
    listElements.forEach(function (li) {
        if (counter % 2 == 0) {
            li.style.background = 'purple';
        }
        else {
            li.style.background = 'blueviolet';
        }
        counter++;
    });
}
backgroundColour();
// Adding a new element to the list
var button = (_a = document.querySelector('#button')) === null || _a === void 0 ? void 0 : _a.addEventListener('click', function (event) {
    event.preventDefault();
    // Creating a new list element with text
    var newListElement = document.createElement('li');
    var textInput = document.querySelector('#text');
    newListElement.innerText = textInput.value;
    if (textInput.value == "") { // If no text was submitted
        newListElement.innerText = "[...]";
    }
    // Add the delete button to the new list element
    newListElement.innerHTML += '<button id="delete">' + 'X' + '</button>';
    // Appending new list element to the end of the list
    var list = document.querySelector('ul');
    list === null || list === void 0 ? void 0 : list.append(newListElement); // .appendChild() works the same
    // Clear the input field
    textInput.value = "";
    // Alternating the colours of the list elements
    backgroundColour();
});
// Deleting an element from the list
var _delete = (_b = document.querySelector('ul')) === null || _b === void 0 ? void 0 : _b.addEventListener('click', function (event) {
    event.preventDefault();
    // If the delete button is clicked remove the list element
    if (event.target.id == 'delete') {
        event.target.parentElement.remove();
    }
    // Alternating the colours of the list elements
    backgroundColour();
});
// Deleting all elements in the list
var clear = (_c = document.querySelector('#clear')) === null || _c === void 0 ? void 0 : _c.addEventListener('click', function (event) {
    event.preventDefault();
    var list = document.querySelector('ul');
    list.innerHTML = "";
});
// // Only show the list elements containing the words searched for
// let filter = document.querySelector('#search')?.addEventListener('keyup', (e: Event) => {
//     e.preventDefault();
//     let text = (e.target as Element).value.toLowerCase();
//     let itemList = document.querySelectorAll('li');
//     itemList = Array.prototype.slice.call(itemList) as Array<String>;
//     itemList.forEach((item: String) => {
//         if(item == text){
//             item.style.display = 'block';
//         }else {
//             item.style.display = 'none';
//         }
//         if(text == ""){
//             item.style.display = 'block';
//         }
//     })
// }) // Fuck typescript
