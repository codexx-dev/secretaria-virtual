import santosBrasao from "../../assets/santos-brasao.png"

function LogoSantos(){
  return(
    <div className="flex flex-col items-center font-bold font-[Roboto]">
      <img className="w-12" src={santosBrasao}></img>
      <span className="text-[0.5rem] text-lime-400 tracking-[2px]">PREFEITURA DE</span>
      <span className="text-4xl text-emerald-600 leading-0 mt-3">Santos</span>
    </div>
  );
}

export default LogoSantos;