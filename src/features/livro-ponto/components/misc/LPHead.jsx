import LogoCidadeEduca from "../../../../components/icons/LogoCidadeEduca";
import LogoSantos from "../../../../components/icons/LogoSantos";

function LPHead({className, title=""}){
  return (
    <div className={`flex justify-between px-8 ${className}`}>
      <LogoSantos/>
      <div className='flex flex-col items-center'>
        <h1 className='text-2xl font-bold'>PREFEITURA DE SANTOS</h1>
        <h2 className='text-xl font-semibold'>Secretaria de Educação</h2>
        <h2 className='text-base font-semibold mt-2'>{title.toUpperCase()}</h2>
      </div>
      <LogoCidadeEduca/>
    </div>
  );
}

export default LPHead;