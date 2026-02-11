import { twMerge } from "tailwind-merge";
import santosBrasao from "../../assets/cidade-educadora.png"

function LogoCidadeEduca({className}){
  return(
    <div className={twMerge("flex flex-row items-center font-bold font-[Roboto] gap-1", className)}>
      <img className="w-12" src={santosBrasao}></img>
      <div className="flex flex-col">
        <span className="text-lg leading-5">Santos</span>
        <span className="text-[0.68rem] leading-2.75">CIDADE <br/> EDUCADORA</span>
      </div>
    </div>
  );
}

export default LogoCidadeEduca;