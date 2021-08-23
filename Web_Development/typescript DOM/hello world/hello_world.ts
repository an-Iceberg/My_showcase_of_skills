// alert("Alert! This is an alert.");
// console.dir(document);
// document.title = "Yer Mum XD";
// console.log(document.head);
// console.log(document.all);
// document.all[12].textContent = "Hello World";
// console.log(document.forms);
// console.log(document.links);
// console.log(document.scripts);
// console.log(document.images);
// ===
// console.log("Domain: " + document.domain);
// console.log("URL: " + document.URL);
// console.log("Title: " + document.title);
// console.log("Doctype: " + document.doctype);
// === get element by id ===
// let listElement = document.getElementById('sample_id') as HTMLUListElement;
// listElement.innerText = 'Hello world';
// listElement.style.borderBottom = '5px dotted brown';
// === get elements by class name ===
// let title = document.getElementsByClassName('header_text');
// console.log(title);
// console.log(title[0]);
// title[0].textContent = 'Yo mama gay';
// title[0].style doesn't work for some reason
// title[0].innerHTML += '<h1 style="font-size: 3rem">Yo mama ded</h1>';
// === get elements by tag name ===
// let listElements = document.getElementsByTagName('li');
// for(let index: number = 0; index < listElements.length; index++){
//     console.log(listElements[index]);
// }
// listElements[0].style.backgroundColor = '#f4f4f4';

// === query selectors ===
// let header = document.querySelector('.header_text') as HTMLHeadingElement;
// header.style.borderBottom = '5px dotted black';
// let input = document.querySelector('#text') as HTMLInputElement;
// input.value = 'Hello World';

// let submit = document.querySelector('input[type="submit"]') as HTMLInputElement;
// submit.value = 'Send';

// let listItem = document.querySelectorAll('li') as NodeListOf<HTMLLIElement>;
// for(let index: number = 0; index < listItem.length; index++){
//     listItem[index].style.backgroundColor = 'black';
// }

// let secondItem = document.querySelector('li:nth-child(2)') as HTMLLIElement;
// secondItem.style.backgroundColor = 'coral';

// let titles = document.querySelectorAll('h2') as NodeListOf<HTMLHeadingElement>;
// for(let index = 0; index < titles.length; index++){
//     titles[index].style.color = 'salmon';
// }

// === Traversing the DOM ===
// let itemList = document.querySelector('ul') as HTMLUListElement;
// console.log(itemList.parentNode?.parentNode);

// if(itemList.parentElement != null){
//     itemList.parentElement.style.backgroundColor = 'wheat';
// }
// === Child nodes ===
// console.log(itemList.childElementCount);
// if(itemList.hasChildNodes()){
//     console.log(itemList.childNodes);
// }
// console.log(itemList.children);
// console.log(itemList.children[1]);
// itemList.children[1].style.background = 'yellow';
// itemList.children[3].style.background = 'black';
// console.log(itemList.firstChild);
// console.log(itemList.firstElementChild);
// if(itemList.firstElementChild != null){
//     itemList.firstElementChild.style.background = 'grey';
// }
// console.log(itemList.firstElementChild?.textContent);
// if(itemList.lastElementChild != null){
//     itemList.lastElementChild.textContent = 'Hello World';
// }

// === Traversing the DOM ===
// let itemList = document.querySelector('ul') as HTMLUListElement;
// console.log(itemList.parentNode?.parentNode);

// if(itemList.parentElement != null){
//     itemList.parentElement.style.backgroundColor = 'wheat';
// }
// === Child nodes ===
// console.log(itemList.childElementCount);
// if(itemList.hasChildNodes()){
//     console.log(itemList.childNodes);
// }
// console.log(itemList.children);
// console.log(itemList.children[1]);
// itemList.children[1].style.background = 'yellow';
// itemList.children[3].style.background = 'black';
// console.log(itemList.firstChild);
// console.log(itemList.firstElementChild);
// if(itemList.firstElementChild != null){
//     itemList.firstElementChild.style.background = 'grey';
// }
// console.log(itemList.firstElementChild?.textContent);
// if(itemList.lastElementChild != null){
//     itemList.lastElementChild.textContent = 'Hello World';
// }
// console.log(itemList.children[0].nextSibling);
// console.log(itemList.children[1].nextElementSibling);
// console.log(itemList.previousElementSibling);
// console.log(itemList.previousSibling);
// if(itemList.previousElementSibling != null){
//     itemList.previousElementSibling.style.color = 'green';}
// let newDiv = document.createElement('div');
// newDiv.className = 'header_text';
// newDiv.id = 'text';
// newDiv.setAttribute('title', 'hello world');

// let newDivText = document.createTextNode('Hello World');
// newDiv.appendChild(newDivText);

// let header = document.querySelector('header');
// let h1 = document.querySelector('.header_text');

// if(header != null && h1 != null){
//     header.insertBefore(newDiv, h1);
// }

// let oddListElems = document.querySelectorAll('li:nth-child(odd') as NodeListOf<HTMLLIElement>; // odd/even <== both work fine
// oddListElems.forEach(li => {
//     li.style.background = 'purple';
// });

// confirm("Confirm please!");