// The old way of doing it
// function buttonClick(event: Event): void {
//     // Creating a new list element with text
//     let newListElement = document.createElement('li');
//     let newListText = document.createTextNode('Seven slender sticks upon the hills of the Scottish highlands');
//     newListElement.appendChild(newListText);
//     newListElement.innerHTML += '<h3>'+event.target?.id+'</h3>';

//     // Appending new list element to the end of the list
//     let list = document.querySelector('ul');    
//     list.appendChild(newListElement);

//     // Alternating the colours of the list elements
//     let oddListElems = document.querySelectorAll('li:nth-child(odd') as NodeListOf<HTMLLIElement>;
//     oddListElems.forEach(li => {
//         li.style.background = 'purple';
//     })

//     console.log('MouseX:'+event.clientX);
//     console.log('MouseY:'+event.clientY);
//     console.log('OffsetX:'+event.offsetX);
//     console.log('OffsetY:'+event.offsetY);

//     console.log('Alt:'+event.altKey);
//     console.log('Ctrl:'+event.ctrlKey);
//     console.log('Shift'+event.shiftKey);
// }

// The new way: event listener
// let button = document.getElementById('buttonTemp')?.addEventListener('click', buttonClick); // a querySelector() works as well

// let button = document.querySelector('#buttonTemp');
// button?.addEventListener('click', runEvent);

// let button = document.querySelector('#buttonTemp');
// // button?.addEventListener('click', runEvent);
// // button?.addEventListener('dblclick', runEvent);
// // button?.addEventListener('mousedown', runEvent);
// // button?.addEventListener('mouseup', runEvent);

// let box = document.querySelector('#box');
// box?.addEventListener('mouseenter', () =>{
//     box!.style.background = 'black';
// })
// box?.addEventListener('mouseleave', () =>{
//     box!.style.background = '#fefefe';
// })
// box?.addEventListener('mouseenter', runEvent);
// box?.addEventListener('mouseleave', runEvent);
// box?.addEventListener('mouseover', runEvent); // fires when you mouse over a different HTML element
// box?.addEventListener('mouseout', runEvent); // fires when you mouse out of a different HTML element

// function runEvent(e: Event){
//     console.log('Event type:'+e.type);
// }

// let output = document.querySelector('#output');

// function runEvent(e: Event){
//     console.log('Event type:'+e.type);

//     output!.innerHTML = '<h3>MouseX:'+e.offsetX+'</h3><h3>MouseY:'+e.offsetY+'</h3>';

    // box!.style.background = 'rgb('+e.offsetX+',0,'+e.offsetY+')';
    // document.body.style.background = 'rgb('+e.offsetX+',0,'+e.offsetY+')';
// }

// let button = document.querySelector('#buttonTemp');
// let textInput = document.querySelector('#text') as HTMLInputElement;

// button?.addEventListener('click', () =>{
//     document.querySelector('.header_text')!.innerText = textInput.value;
// })

// textInput?.addEventListener('keydown', runEvent);
// textInput?.addEventListener('keyup', runEvent);
// textInput?.addEventListener('keypress', runEvent);

// textInput?.addEventListener('focus', runEvent); // fires when you select an HTML input element
// textInput?.addEventListener('blur', runEvent); // fires when you deselect an HTML input element

// textInput?.addEventListener('cut', runEvent);
// textInput?.addEventListener('copy', runEvent);
// textInput?.addEventListener('paste', runEvent);

// textInput?.addEventListener('input', runEvent);

// let selector = document.querySelector('#numberSelect');

// selector?.addEventListener('change', () => {
//     console.log(selector!.value);
// })

// let form = document.querySelector('form')?.addEventListener('submit', runEvent);

// function runEvent(e: Event){
//     e.preventDefault();
//     console.log(e.type);
// }

// let button = document.querySelector('#buttonTemp')?.addEventListener('click', buttonClick);

// let textInput = document.querySelector('#text') as HTMLInputElement;
// let selector = document.querySelector('#numberSelect');

// selector?.addEventListener('change', () => {
//     console.log(selector!.value);
// })

// let form = document.querySelector('form')?.addEventListener('submit', runEvent);

// function runEvent(e: Event){
//     e.preventDefault();
//     console.log(e.type);
// }

// Convert HTML list into array
// Array.from(items).forEach(function(item){
//     let itemName = item.firstChild.textContent;
// }