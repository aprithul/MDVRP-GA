var canvas = document.querySelector('canvas');
canvas.width = window.innerWidth*0.54;
canvas.height = canvas.width/2;
var c = canvas.getContext('2d');


function clear_canvas()
{
    c.clearRect(0,0,canvas.width, canvas.height);
    console.log("cleared");
}

function draw_node(x,y,is_depot)
{
    c.beginPath();
    if(is_depot)
        c.arc( x,y, 10, 0, 2*Math.PI, true);
    else
        c.arc( x,y, 4, 0, 2*Math.PI, true);

    c.stroke();
}

function draw_edge(x1,y1,x2,y2)
{
    c.beginPath()
    c.moveTo(x1,y1);
    c.lineTo(x2,y2);
    c.stroke();
}
