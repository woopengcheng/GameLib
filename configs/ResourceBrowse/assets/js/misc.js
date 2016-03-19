var filesList = new Array();
var filesIndex = 0;
var fso = new ActiveXObject("Scripting.FileSystemObject");
function searchFilesList(filePath){
    var f = fso.GetFolder(filePath);
    // 遍历目录
    var fk = new Enumerator(f.SubFolders);
    for (; !fk.atEnd(); fk.moveNext()) {
        filesList[filesIndex++] = fk.item();
        searchFilesList(fk.item());
    }
    // 遍历目录文件
    var fc = new Enumerator(f.files);
    for (; !fc.atEnd(); fc.moveNext()) {
        filesList[filesIndex++] = fc.item();
    }
}
function searchFiles(){
    searchFilesList(fixfolder.value);
    // 循环信息信息
    for (var i=0; i<filesList.length; i++){
        textarea.innerHTML += filesList[i]+"<br>";
    }
}