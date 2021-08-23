// Alternating the colours of the list elements
function backgroundColour(): void {
    let counter = 0;
    let listElements = document.querySelectorAll('li');

    listElements.forEach(li => {
        if (counter % 2 == 0) {
            li.style.background = 'purple';
        } else {
            li.style.background = 'blueviolet';
        }
        counter++;
    });
}

backgroundColour();

// Adding a new element to the list
let button = document.querySelector('#button')?.addEventListener('click', (event: Event): void => {
    event.preventDefault();

    // Creating a new list element with text
    let newListElement = document.createElement('li');
    let textInput = document.querySelector('#text') as HTMLInputElement;
    newListElement.innerText = textInput.value;
    if (textInput.value == "") { // If no text was submitted
        newListElement.innerText = "[...]";
    }

    // Add the delete button to the new list element
    newListElement.innerHTML += '<button id="delete">' + 'X' + '</button>';

    // Appending new list element to the end of the list
    let list = document.querySelector('ul');
    list?.append(newListElement); // .appendChild() works the same

    // Clear the input field
    textInput.value = "";

    // Alternating the colours of the list elements
    backgroundColour();
});

// Deleting an element from the list
let _delete = document.querySelector('ul')?.addEventListener('click', (event: Event): void => {
    event.preventDefault();

    // If the delete button is clicked remove the list element
    if ((event.target as Element).id == 'delete') {
        (event.target as Element).parentElement!.remove();
    }

    // Alternating the colours of the list elements
    backgroundColour();
});

// Deleting all elements in the list
let clear = document.querySelector('#clear')?.addEventListener('click', (event: Event) => {
    event.preventDefault();

    let list = document.querySelector('ul');
    list!.innerHTML = "";
})

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
// }) // &%*# typescript
