import { has31Days, getWeekday, getMonthName, getWeekName, isoTime } from "../../../utils"
import LogoSantos from '../../../components/icons/LogoSantos';
import LogoCidadeEduca from '../../../components/icons/LogoCidadeEduca';
import { feriados2025 } from '../feriados2025';
import LPHead from './LPHead';

function LPAdmPage() {
  //informações gerais
  const refYear = 2025;
  const refMonth = 11;
  const lastMonth = (refMonth - 1 < 0)? 11 : refMonth - 1;
  const lastYear = (refMonth === 0)? refYear - 1 : refYear;
  const umeName = "ume monte cabrão";
  const pageNote = "Substituindo o substituto";
  const pageNumber = 1;
  
  ////informações pessoais
  const person = {
    nome: "Álvaro Esteves Matos",
    registro: "40.558-9",
    regime: "Estatutário",
    cargo: "Secretário de Unidade Escolar",
    jornada: "200",
    horario: "Todos os dias sem excessão",
  }; 

  return (
    <div className="lpp size-full flex flex-col">

      <LPHead title={umeName.toUpperCase()}/>
      
      <span className='font-semibold text-right w-full'>{`página ${pageNumber}`}</span>

      <span className='text-center underline mb-1'>{`REGISTRO DE FREQUÊNCIA — 26/${String(lastMonth).padStart(2, '0')}/${lastYear} A 25/${String(refMonth).padStart(2, '0')}/${refYear}`} </span>

      <table className='table-auto border-collapse'>
        <colgroup>
          <col className='w-0'/>
          <col className='w-[5%]'/>
          <col className='w-[5%]'/>
          <col className='w-auto'/>
          <col className='w-0'/>
          <col className='w-0'/>
          <col className='w-0'/>
          <col className='w-0'/>
        </colgroup>
        <tr className='*:pr-2'>
          <td className='font-semibold' >Nome:</td>
          <td colSpan={3}>{person.nome.toUpperCase()}</td>
          <td className='font-semibold'>Registro:</td>
          <td>{person.registro}</td>
          <td className='font-semibold'>Regime:</td>
          <td>{person.regime}</td>
        </tr>
        <tr className='*:pr-2'>
          <td className='font-semibold'>Cargo:</td>
          <td colSpan={4}>{person.cargo}</td>
          <td className='font-semibold'>Jornada:</td>
          <td colSpan={2}>{person.jornada + 'h'}</td>
        </tr>
        <tr className='*:pr-2'>
          <td className='font-semibold'>Horário:</td>
          <td colSpan={7}>{person.horario}</td>
        </tr>
        <tr className='text-center font-semibold text-white border-t-2 border-x-2 border-black bg-cyan-600 *:px-2 *:border *:border-black'>
          <td>MÊS</td>
          <td colSpan={2}>Dias</td>
          <td colSpan={2}>Assinaturas</td>
          <td>Natureza da Falta</td>
          <td colSpan={2}>Observação</td>
        </tr>

        {Array.from({length:6}).map((_, idx)=>{
          const day = 26 + idx;
          const weekday = getWeekday(day, lastMonth, lastYear);
          const weekName = getWeekName(weekday);
          const dayName = feriados2025[isoTime(lastYear, lastMonth, day)] || ((weekday === 0 || weekday === 6)? weekName : null);
          const isWorkDay = dayName === null;

          return (
            <tr className={`${(!isWorkDay)?'bg-neutral-400':''}
            text-center text-sm border-x-2 *:border *:border-black`} key={idx}>
              
              {(idx === 0)?
                <td className='font-semibold text-white bg-cyan-600' rowSpan={6}>
                  <span className='text-md [writing-mode:vertical-lr] -rotate-180'>{getMonthName(lastMonth).toUpperCase()}</span>
                </td>
              : ''}

              {(day === 31 && has31Days(lastMonth, lastYear))?
                <td className='bg-neutral-400 select-none' colSpan={8}> &nbsp; </td>
                :
                <>
                  <td>{day}</td>
                  <td>{weekName[0]}</td>
                  <td colSpan={2}>
                    <div className='grid grid-cols-2 gap-px bg-black'>
                      <div className={`${(!isWorkDay)?'bg-neutral-400':'bg-white'}`}>
                        &nbsp;{dayName}&nbsp;
                      </div>
                      <div className={`${(!isWorkDay)?'bg-neutral-400':'bg-white'}`}>
                        &nbsp;{dayName}&nbsp;
                      </div>
                    </div>
                  </td>
                  <td></td>
                  <td colSpan={2}></td>
                </>
              }
            </tr>
          );
        })}

        {Array.from({length:25}).map((_, idx)=>{
          const day = 1 + idx;
          const weekday = getWeekday(day, refMonth, refYear);
          const weekName = getWeekName(weekday);
          const dayName = feriados2025[isoTime(refYear, refMonth, day)] || ((weekday === 0 || weekday === 6)? weekName : null);
          const isWorkDay = dayName === null;


          return (
            <tr className={`${(!  isWorkDay)?'bg-neutral-400':''} text-center text-sm
              ${(day === 1)? 'border-t-2': ''}
              ${(day === 25)? 'border-b-2': ''}
              border-x-2 *:border *:border-black`} key={idx}>
              
              {(idx === 0)?
                <td className='font-semibold text-white bg-cyan-600' rowSpan={25}>
                  <span className='text-md [writing-mode:vertical-lr] -rotate-180'>{getMonthName(refMonth).toUpperCase()}</span>
                </td>
              : ''}

              <>
                <td>{day}</td>
                <td>{weekName[0]}</td>
                <td colSpan={2}>
                    <div className='grid grid-cols-2 gap-px bg-black'>
                      <div className={`${(!isWorkDay)?'bg-neutral-400':'bg-white'}`}>
                        &nbsp;{dayName}&nbsp;
                      </div>
                      <div className={`${(!isWorkDay)?'bg-neutral-400':'bg-white'}`}>
                        &nbsp;{dayName}&nbsp;
                      </div>
                    </div>
                </td>
                <td></td>
                <td colSpan={2}></td>
              </>

            </tr>
          );
        })}
      </table>

      <div>{`Nota: ${pageNote}`}</div>

      <div className='mt-auto mb-5 flex justify-between'>
        <div className='flex flex-col items-center'>
          <span>_____________________________________</span>
          <span>Secretário de Unidade Escolar</span>
        </div>
        <div className='flex flex-col items-center'>
          <span>_____________________________________</span>
          <span>Diretor de Unidade Escolar</span>
        </div>
      </div>
    </div>
  )
}

export default LPAdmPage
