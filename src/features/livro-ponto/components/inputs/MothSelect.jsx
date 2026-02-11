import { getMonthName } from "../../../../utils";

function MonthSelect(){
  return(
    <select className="ml-1 text-black">
      {Array.from({length:12}).map((_, idx)=>{
        const month1 = getMonthName(idx + 1).substring(0, 3).toLocaleUpperCase();
        const month2 = getMonthName(idx + 2).substring(0, 3).toLocaleUpperCase();
        const date = new Date();
        const selMonth = (date.getMonth() + (date.getDate() > 25)? 1 : 0);
        return(
          <option key={idx} selected={selMonth === (idx)}>
            {`${month1}/${month2}`}
          </option>
        );
      })}
    </select>
  );
}

export default MonthSelect;