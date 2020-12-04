var canvas = document.querySelector('canvas');
canvas.width = window.innerWidth*0.2;
canvas.height = window.innerWidth*0.2;
var c = canvas.getContext('2d');
c.lineWidth = 1;
c.imageSmoothingEnabled=true;
var min_x = 9999999;
var min_y = 9999999;
var max_x = -9999999;
var max_y = -9999999;
var scaling_factor = 1;
clear_canvas();

function clear_canvas()
{
    c.fillStyle = "white";
    c.fillRect(0,0,canvas.width, canvas.height);
}

function draw_node(x,y,is_depot)
{
    if(x<min_x)min_x = x;
    if(y<min_y)min_y = y;
    if(x>max_x)max_x = x;
    if(y>max_y)max_y = y;

    var x_scale = 1;
    var y_scale = 1;
    if(max_x-min_x > 1)
        x_scale = canvas.width*0.8/(max_x-min_x);
    if(max_y-min_y >1)
        y_scale = canvas.height*0.8/(max_y-min_y);

    scaling_factor = Math.min(x_scale,y_scale);

    c.beginPath();
    
    if(is_depot){
        c.strokeStyle = "#FF0000";
        c.fillStyle = "#FF0000";
        c.arc( x*scaling_factor,y*scaling_factor, 5, 0, 2*Math.PI, true);
    }
    else{
        c.strokeStyle = "#0000FF";
        c.fillStyle = "#0000FF";
        c.arc( x*scaling_factor,y*scaling_factor,  2, 0, 2*Math.PI, true);
    }

    c.fill();
}

function draw_edge(x1,y1,x2,y2)
{
    c.strokeStyle = "#000000";
    c.beginPath()
    c.moveTo(x1* scaling_factor,y1*scaling_factor);
    c.lineTo(x2*scaling_factor,y2*scaling_factor);
    c.stroke();
}


