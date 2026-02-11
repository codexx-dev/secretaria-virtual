import { useEffect, useRef, useState } from "react";

const dvdColors = [
  '#ffff00', //amarelo
  '#ff00ff', //magenta
  '#00ffff', //ciano
  '#ff8000', //laranja
  '#8000ff', //roxo
];

function Page404(){
  const speed = 4;

  const [currCol, setCurrCol] = useState(0);
  const [position, setPosition] = useState({x: 0, y:0});
  const [direction, setDirection] = useState({ x: 1, y: 1 });

  const objSize = useRef({x:228, y:168});
  const screenSize = useRef({x:1000, y:1000})

  const handleResize = ()=> {
    screenSize.current.x = document.documentElement.clientWidth;
    screenSize.current.y = document.documentElement.clientHeight;
  };

  const handlePingPong = ()=>{
    setPosition({
      x: position.x + direction.x * speed,
      y: position.y + direction.y * speed
    });

    if(position.x > (screenSize.current.x - objSize.current.x) || position.x < 0){
      setPosition({x: position.x - direction.x * speed, y: position.y}); 
      setDirection({x: -direction.x, y: direction.y});

      setCurrCol((currCol + 1) % dvdColors.length);
    }

    if(position.y > (screenSize.current.y - objSize.current.y) || position.y < 0){
      setPosition({x: position.x, y: position.y - direction.y * speed}); 
      setDirection({x: direction.x, y: -direction.y});

      setCurrCol((currCol + 1) % dvdColors.length);
    }
  }

  useEffect(()=>{
    handleResize();
    window.addEventListener('resize', handleResize);
    return () => {window.removeEventListener('resize', handleResize)};
  }, []);

  useEffect(()=>{
    const timeOut = setTimeout(()=>{
      requestAnimationFrame(handlePingPong);
    }, 100);

    return () => clearTimeout(timeOut);

  }, [position]);

  //console.log(position, direction);

  return (
    <div className="absolute top-0 left-0 w-screen h-screen bg-neutral-900 text-neutral-50 overflow-hidden">
      <div className="absolute flex flex-col items-center gap-2" style={{left: position.x, top: position.y, color: dvdColors[currCol]}}>
        <span className="text-9xl font-bold underline">404</span>
        <span className="text-2xl font-bold">Page Not Found</span>
      </div>
    </div>
  );
}

export default Page404;