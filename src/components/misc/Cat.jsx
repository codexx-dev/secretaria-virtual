import { twMerge } from "tailwind-merge";
import { useState } from "react";
import cat from "../../assets/cat-spinning.gif";

function Cat({className}){
  const [count, setCount] = useState(1);

  const handleClick = ()=> {
    setCount(count + 1);
  };

  return (
    <div className={twMerge('flex flex-row flex-wrap justify-center gap-2 select-none cursor-pointer', className)}
      onClick={handleClick}
    >
      {Array.from({length: count}).map((_, idx)=>{
        return (
          <div className="bg-neutral-50 w-60 h-80 rounded-lg flex">
            <img className="w-80 my-auto" key={idx} src={cat}/>
          </div>
        );
      })}

    </div>
  );
}

export default Cat;