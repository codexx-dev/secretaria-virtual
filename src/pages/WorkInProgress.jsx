import { useEffect, useState } from "react";

function WorkInProgress(){
  const [animText, setAnimText] = useState('');

  const handleAnim = ()=>{
    setAnimText((animText.length === 5)? '' : animText + '.');
    console.log(animText);
  };

  useEffect(()=>{
    const timeOut = setTimeout(handleAnim, 500);
    return ()=> {clearTimeout(timeOut)};
  }, [animText]);

  return(
    <div className="py-10 flex flex-col gap-10 bg-slate-800 text-neutral-50">
      <h1 className="text-center text-4xl font-semibold">
        TRABALHO EM PROGRESSO
        <span className="absolute text-4xl font-semibold">{animText}</span>
      </h1>
    </div>
  );
}

export default WorkInProgress;