import { useEffect, useState } from "react";
import LogoCidadeEduca from "../../../../components/icons/LogoCidadeEduca";
import LogoSantos from "../../../../components/icons/LogoSantos";
import { verticalTextBr } from "../../../../utils";
import { makeEmptCrachaInfo } from "../../stores/crachaStore";

function Cracha({empt, info}){
  const [imgUrl, setImgUrl] = useState();

  useEffect(()=>{
    if(!info?.photo) return;
    
    const url = URL.createObjectURL(info.photo);
    setImgUrl(url);

    //debug
    //console.log(info.photo);
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  return (
    <div className="*:w-50 *:h-80 *:px-2 *:border-3 *:text-center *:flex *:flex-col *:gap-2 [&_h1,h2]:font-bold">
      
      <div>
        {!empt &&
        <>
          <h1>{info.ume}</h1>
          <span className="my-2 text-[10pt]">{"(13) 3352-4991"}</span>

          <div className="flex flex-row items-center relative">
            <span className="ml-2 whitespace-break-spaces absolute font-bold text-[14pt] text-red-500">
              {verticalTextBr(info.refYear)}
            </span>

            <div className="w-25 h-30 mx-auto border flex items-center">
              <img src={imgUrl}></img>
            </div>
          </div>


          <div className="border grow">
            <h1>{info.name}</h1>
          </div>

          <span className="py-1 font-bold text-[8pt] leading-3">O uso deste crachá é obrigatório para utilizar o transporte.</span>
        </>}
      </div>
          
      <div className="relative rotate-180">
        {!empt &&
        <>
          <div className="flex flex-row items-center scale-35 absolute -left-38 -top-4">
            <LogoSantos className="scale-80"/>
            <div className="min-w-max mr-5 font-extrabold text-2xl leading-4">
              <h1>PREFEITURA DE SANTOS</h1>
              <h2 className="text-lg">Secretaria de Educação</h2>
            </div>
            <LogoCidadeEduca/>
          </div>

          <div className="text-[8pt] font-bold">
            <div className="mt-8 flex flex-row justify-center">
              <h2>{`Ano: ${info.refYear} - Turma:`}</h2>
              <span className="w-10 border-b" >{info.refClass}</span>
            </div>
            <h2 className="">Autorizados a buscar no ponto:</h2>

            <ol className="list-decimal list-inside text-left *:my-2">
              {Array.from({length:7}).map((_, idx)=>{
                const list = info.parentList.split(';');
                let text = '';
                if(list[0 + idx]) text += String(list[0 + idx]);
                text += '  ';
                if(list[7 + idx]) text += String(list[7 + idx]);
                return (
                  <li key={idx}>
                    <span>{text}</span>
                    <hr className="ml-3"/>
                  </li>
                );
              })}
            </ol>

            <h2 className="text-left">Telefones:</h2>
            <span>{info.tel}</span>
            <hr/>
            <h2 className="text-left">Responsável:</h2>
            <span>{info.mainParent}</span>
          </div>
        </>}
      </div>
    </div>

    
  );
}

export default Cracha;